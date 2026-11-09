#include "jit_executor.hpp"

#include <bit_ops.hpp>

#define INCR_PTR data.cc.add  (data.pc_val, sizeof(Word))


void JitExec::execute_next_block() {
  Register start_pc = pc();

  auto bt_found = bt_cache_.find(start_pc);

  if (bt_found != bt_cache_.end()) {
    bt_found->second(cpu_, guest_ptr(0, kMemorySize));
    return;
  }

  auto bb_found = bb_cache_.find(start_pc);

  if (bb_found == bb_cache_.end()) {
    BasicBlockTemp block;

    block.instructions = decode_next_block();

    bb_found = bb_cache_.emplace(
      start_pc, std::move(block)
    ).first;
  }

  auto& entry = bb_found->second;

  if (entry.exec_cnt >= kThreshold) {
    JitFunc func = translate(entry.instructions);

    bt_cache_.emplace(start_pc, func);
    func(cpu_, guest_ptr(0, kMemorySize));
  } else {
    entry.exec_cnt++;
    interpret(entry.instructions);
  }
}

JitExec::JitFunc JitExec::translate(const BasicBlock &block) {
  asmjit::CodeHolder code;

  code.init(runtime_.environment(), runtime_.cpu_features());

  asmjit::x86::Compiler cc(&code);

  auto* func = cc.add_func(
      asmjit::FuncSignature::build<BlckExitCode, CoreState*>());

  JitData data{
    cc,
    cc.new_gp_ptr(),
    cc.new_gp_ptr(),
    cc.new_gp32(),
    cc.new_gp32(),
    cc.new_gp64(),
    cc.new_gp32(),
    cc.new_gp32()
  };

  func->set_arg(0, data.state);
  func->set_arg(1, data.ram);

  cc.mov(data.pc_val, pc_to_dword_ptr(data.state));
  cc.mov(data.status, kSuccess);

  for (const Instr& instr : block) {
    translate_instr(instr, data);
  }

  cc.mov(pc_to_dword_ptr(data.state), data.pc_val);
  cc.ret(data.status);

  cc.end_func();
  cc.finalize();

  JitFunc entry = NULL;

  runtime_.add(&entry, &code);

  return entry;
}

void JitExec::interpret(const BasicBlock &block) {
  for (const Instr& instr : block) {
    executor(instr);
  }
}

asmjit::x86::Mem JitExec::reg_to_dword_ptr(const asmjit::x86::Gp& state,
                                           std::size_t            index) {
  return asmjit::x86::dword_ptr(
    state,
    CoreState::reg_offset(index)
  );
}

asmjit::x86::Mem JitExec::pc_to_dword_ptr(const asmjit::x86::Gp& state) {
  return asmjit::x86::dword_ptr(
    state,
    CoreState::pc_offset()
  );
}

void JitExec::translate_instr(const Instr   &instr, 
                              const JitData &data) {
  switch(instr.instr) {
    case kClz:
      translate_clz(instr, data);
      break;
    case kLi:
      translate_li(instr, data);
      break;
    case kSysc:
      translate_sysc(instr, data);
      break;
    case kSt:
      translate_st(instr, data);
      break;
    case kStp:
      translate_stp(instr, data);
      break;
    case kBne:
      translate_bne(instr, data);
      break;
    case kBeq:
      translate_beq(instr, data);
      break;
    case kSelc:
      translate_selc(instr, data);
      break;
    case kSti:
      translate_sti(instr, data);
      break;
    case kJ:
      translate_j(instr, data);
      break;
    case kSsat:
      translate_ssat(instr, data);
      break;
    case kLd:
      translate_ld(instr, data);
      break;
    case kSbit: 
      translate_sbit(instr, data);
      break;
    case kAdd: 
      translate_add(instr, data);
      break;
    case kAddi: 
      translate_addi(instr, data);
      break;
  }
}

void JitExec::translate_clz(const Instr &instr, const JitData &data) {
  data.cc.mov  (data.tmp1, reg_to_dword_ptr(data.state, instr.field1));
  data.cc.lzcnt(data.tmp1, data.tmp1);
  data.cc.mov  (reg_to_dword_ptr(data.state, instr.field2), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_li(const Instr &instr, const JitData &data) {
  Word imm = sgn_extend(instr.field3, FIELD3_WIDTH(instr.instr));

  data.cc.mov(data.tmp1, imm);
  data.cc.mov(reg_to_dword_ptr(data.state, instr.field1), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_sysc(const Instr &instr, const JitData &data) {
  data.cc.mov(data.status, kSyscall);

  INCR_PTR;
}

void JitExec::translate_st(const Instr &instr, const JitData &data) {
  Word imm = sgn_extend(
      instr.field3, FIELD3_WIDTH(instr.instr));

  SignedWord offset = std::bit_cast<SignedWord>(imm);

  data.cc.mov(data.addr.r32(), reg_to_dword_ptr(data.state, instr.field1));
  data.cc.add(data.addr, offset);
  data.cc.mov(data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.mov(asmjit::x86::dword_ptr(data.ram, data.addr), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_stp(const Instr &instr, const JitData &data) {
  std::uint32_t field4_mask = get_mask(FIELD4_WIDTH(instr.instr));
  Word          field4      = (instr.field3 >> FIELD3_WIDTH(instr.instr)) 
                            & field4_mask;

  std::uint32_t field3_mask = get_mask(FIELD3_WIDTH(instr.instr));
  Word          field3      = instr.field3 & field3_mask;

  std::uint32_t bit_width = FIELD4_WIDTH(instr.instr);
  Word          extd_imm  = sgn_extend  (field4, bit_width);

  data.cc.mov(data.addr.r32(), reg_to_dword_ptr(data.state, instr.field1));
  data.cc.add(data.addr.r32(), extd_imm);
  data.cc.mov(data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.mov(asmjit::x86::dword_ptr(data.ram, data.addr), data.tmp1);
  data.cc.mov(data.tmp1, reg_to_dword_ptr(data.state, field3));
  data.cc.mov(asmjit::x86::dword_ptr(
                data.ram, data.addr, 0, 
                sizeof(Word)), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_bne(const Instr &instr, const JitData &data) {
  Word offset = sgn_extend(instr.field3, FIELD3_WIDTH(instr.instr)) << 2;

  asmjit::Label not_taken = data.cc.new_label();

  data.cc.mov (data.tmp1, reg_to_dword_ptr(data.state, instr.field1));
  data.cc.cmp (data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.je  (not_taken);
  data.cc.add (data.pc_val, offset);
  data.cc.bind(not_taken);
  INCR_PTR;
}

void JitExec::translate_beq(const Instr &instr, const JitData &data) {
  Word offset = sgn_extend(instr.field3, FIELD3_WIDTH(instr.instr)) << 2;

  asmjit::Label not_taken = data.cc.new_label();

  data.cc.mov (data.tmp1, reg_to_dword_ptr(data.state, instr.field1));
  data.cc.cmp (data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.jne (not_taken);
  data.cc.add (data.pc_val, offset);
  data.cc.bind(not_taken);

  INCR_PTR;
}

void JitExec::translate_selc(const Instr &instr, const JitData &data) {
  data.cc.mov  (data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.mov  (data.tmp2, reg_to_dword_ptr(data.state, instr.field3));
  data.cc.cmp  (data.tmp1, data.tmp2);
  data.cc.cmovb(data.tmp1, data.tmp2);
  data.cc.mov  (reg_to_dword_ptr(data.state, instr.field1), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_sti(const Instr &instr, const JitData &data) {
  Word imm = sgn_extend(instr.field3, FIELD3_WIDTH(instr.instr));

  SignedWord offset = std::bit_cast<SignedWord>(imm);

  data.cc.mov(data.addr.r32(), reg_to_dword_ptr(data.state, instr.field1));
  data.cc.add(data.addr, offset);
  data.cc.mov(data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.mov(asmjit::x86::dword_ptr(data.ram, data.addr), data.tmp1);
  data.cc.mov(reg_to_dword_ptr(data.state, instr.field1), data.addr.r32());

  INCR_PTR;
}

void JitExec::translate_j(const Instr &instr, const JitData &data) {
  data.cc.and_(data.pc_val, 0xF0000000u);
  data.cc.or_ (data.pc_val, instr.field3 << 2);
}

void JitExec::translate_ssat(const Instr &instr, const JitData &data) {
  Word width = instr.field3;

  if (width == 0 || width > kWordSize)
    throw std::out_of_range("jit: ssat: invalid width");

  Word magnitude = Word{1} << (width - 1);
  Word lower = Word{0} - magnitude;
  Word upper = magnitude - 1;

  data.cc.mov  (data.tmp1,reg_to_dword_ptr(data.state, instr.field1));
  data.cc.mov  (data.tmp2, lower);
  data.cc.cmp  (data.tmp1, data.tmp2);
  data.cc.cmovl(data.tmp1, data.tmp2);
  data.cc.mov  (data.tmp2, upper);
  data.cc.cmp  (data.tmp1, data.tmp2);
  data.cc.cmovg(data.tmp1, data.tmp2);
  data.cc.mov  (reg_to_dword_ptr(data.state, instr.field1), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_ld(const Instr &instr, const JitData &data) {
  Word imm = sgn_extend(instr.field3, FIELD3_WIDTH(instr.instr));

  SignedWord offset = std::bit_cast<SignedWord>(imm);

  data.cc.mov(data.addr.r32(), reg_to_dword_ptr(data.state, instr.field1));
  data.cc.add(data.addr, offset);
  data.cc.mov(data.tmp1, asmjit::x86::dword_ptr(data.ram, data.addr));
  data.cc.mov(reg_to_dword_ptr(data.state, instr.field2), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_sbit(const Instr &instr, const JitData &data) {
  Word value = Word{1} << instr.field3;

  data.cc.mov(reg_to_dword_ptr(data.state, instr.field1), value);

  INCR_PTR;
}

void JitExec::translate_add(const Instr &instr, const JitData &data) {
  data.cc.mov(data.tmp1, reg_to_dword_ptr(data.state, instr.field2));
  data.cc.add(data.tmp1, reg_to_dword_ptr(data.state, instr.field3));
  data.cc.mov(reg_to_dword_ptr(data.state, instr.field1), data.tmp1);

  INCR_PTR;
}

void JitExec::translate_addi(const Instr &instr, const JitData &data) {
  Word imm = sgn_extend(instr.field3, FIELD3_WIDTH(instr.instr));

  data.cc.mov(data.tmp1, reg_to_dword_ptr(data.state, instr.field1));
  data.cc.add(data.tmp1, imm);
  data.cc.mov(reg_to_dword_ptr(data.state, instr.field2), data.tmp1);

  INCR_PTR;
}
