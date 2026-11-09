#include "jit_executor.hpp"

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
    cc.new_gp32(),
    cc.new_gp32(),
    cc.new_gp32()
  };

  func->set_arg(0, data.state);

  cc.mov(data.pc_val, pc_to_dword_ptr(data.state));
  cc.mov(data.status, kSuccess);

  for (const Instr& instr : block) {
    translate_instr(instr, data);
  }

  cc.mov     (pc_to_dword_ptr(data.state), data.pc_val);
  cc.ret     (data.status);
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
  
  INCR_PTR;
}

void JitExec::translate_sysc(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_st(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_stp(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_bne(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_beq(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_selc(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_sti(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_j(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_ssat(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_ld(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_sbit(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_add(const Instr &instr, const JitData &data) {
  INCR_PTR;
}

void JitExec::translate_addi(const Instr &instr, const JitData &data) {
  INCR_PTR;
}
