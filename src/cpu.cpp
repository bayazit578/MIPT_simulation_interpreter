#include "cpu.hpp"

#define GET_OPCODE(instr_code, instr)                     \
  Byte opcode{0};                                         \
  Byte mask6 = (Byte)get_mask(6);                         \
  if (((instr_code & (mask6 << (kWordSize - 6)))) == 0) { \
    opcode = (Byte)(instr_code & mask6);                  \
  } else {                                                \
    opcode = (Byte)(instr_code >> (kWordSize - 6));       \
  }                                                       \
  instr.opc = (Opcode)opcode;

#define FIELD1_OFFSET(instr_numb) oper_info[instr_numb].field1.offset
#define FIELD2_OFFSET(instr_numb) oper_info[instr_numb].field2.offset
#define FIELD3_OFFSET(instr_numb) oper_info[instr_numb].field3.offset
#define FIELD4_OFFSET(instr_numb) oper_info[instr_numb].field4.offset
#define FIELD1_WIDTH(instr_numb)  oper_info[instr_numb].field1.width
#define FIELD2_WIDTH(instr_numb)  oper_info[instr_numb].field2.width
#define FIELD3_WIDTH(instr_numb)  oper_info[instr_numb].field3.width
#define FIELD4_WIDTH(instr_numb)  oper_info[instr_numb].field4.width

#define GET_ALL_FIELDS(instr_numb) {                                      \
  std::uint32_t mask_field1 = get_mask(FIELD1_WIDTH(instr_numb));         \
  std::uint32_t mask_field2 = get_mask(FIELD2_WIDTH(instr_numb));         \
  std::uint32_t mask_field3 = get_mask(FIELD3_WIDTH(instr_numb));         \
  std::uint32_t mask_field4 = get_mask(FIELD4_WIDTH(instr_numb));         \
                                                                          \
  instr.field1 = (instr_code >> FIELD1_OFFSET(instr_numb)) & mask_field1; \
  instr.field2 = (instr_code >> FIELD2_OFFSET(instr_numb)) & mask_field2; \
  instr.field3 = (instr_code >> FIELD3_OFFSET(instr_numb)) & mask_field3; \
                                                                          \
  std::uint32_t field4 = (instr_code >> FIELD4_OFFSET(instr_numb))        \
                         & mask_field4;                                   \
  instr.field3 |= field4 << FIELD3_WIDTH(instr_numb);                     \
                                                                          \
  instr.instr = instr_numb;                                               \
  break;                                                                  \
}

#define INCR_PC cpu->set_pc(cpu->get_pc() + sizeof(Word))

#define IS_CTRL(instr)          \
    (instr.instr == kJ          \
  || instr.instr == kBne        \
  || instr.instr == kBeq        \
  || instr.instr == kSysc)      \
 

static SignedWord    sgn_extend(Word value, Word bit_width);
static std::uint32_t get_mask  (std::uint32_t mask_width);
static Word          sgn_sat(Word reg, Word width);


static SignedWord sgn_extend(Word value, Word bit_width) {
  Word m = 1u << (bit_width - 1);
  return std::bit_cast<SignedWord>((value ^ m) - m);
}

static std::uint32_t get_mask(std::uint32_t mask_width) {
  return ((1 << mask_width) - 1);
}

static Word sgn_sat(Word reg, Word width) {
  SignedWord lower_b = -(1 << (width-1));
  SignedWord upper_b = (1 << (width-1)) - 1;

  SignedWord res = std::clamp<SignedWord>(
    std::bit_cast<SignedWord>(reg), 
    lower_b, 
    upper_b
  );

  return std::bit_cast<Word>(res);
}


Register Cpu::prefetch_basic_block(BasicBlock& blk) {
  Register begin_pc = cpu->get_pc();
  Instr instr{};

  do {
    instr = decoder(fetch_instr());
    blk.push_back(instr);
    INCR_PC;
  } while (!IS_CTRL(instr));

  cpu->set_pc(begin_pc);
  return begin_pc;
}


void Cpu::execute_clz(Instr instr) {
  std::uint32_t value = cpu->get_reg(instr.field2);
  std::uint32_t mask  = 1 << (kWordSize - 1);
  std::uint32_t i     = 0;

  for (; i < kWordSize && (value & mask) == 0; i++) {
    value <<= 1;
  }

  cpu->set_reg(instr.field1, i);

  INCR_PC;
}

void Cpu::execute_li(Instr instr) {
  std::uint32_t bit_width = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_imm  = sgn_extend(instr.field3, bit_width);
  cpu->set_reg(instr.field1, (Register)extd_imm);

  INCR_PC;
}

void Cpu::execute_sysc(Instr instr) {
  Syscall sysc;
  sysc.num = cpu->get_reg(kSyscNumReg);

  for (std::uint8_t i = 0; i < kSyscArgNum; i++) {
    sysc.args[i] = cpu->get_reg(i);
  }

  INCR_PC;
  throw sysc;
}

void Cpu::execute_st(Instr instr) {
  std::uint32_t bit_width = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_imm  = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = cpu->get_reg(instr.field1) + extd_imm;
  memory_.store<Word>(addr, cpu->get_reg(instr.field2));

  INCR_PC;
}

void Cpu::execute_stp(Instr instr) {
  std::uint32_t field4_mask = get_mask(FIELD4_WIDTH(instr.instr));
  Word          field4      = (instr.field3 >> FIELD3_WIDTH(instr.instr)) 
                            & field4_mask;

  std::uint32_t field3_mask = get_mask(FIELD3_WIDTH(instr.instr));
  Word          field3      = instr.field3 & field3_mask;

  std::uint32_t bit_width = FIELD4_WIDTH(instr.instr);
  SignedWord    extd_imm  = sgn_extend  (field4, bit_width);

  std::size_t addr = cpu->get_reg(instr.field1) + extd_imm;
  memory_.store<Word>(addr    , cpu->get_reg(instr.field2));
  memory_.store<Word>(addr + 4, cpu->get_reg(field3));

  INCR_PC;
}

void Cpu::execute_bne(Instr instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);
  Word          target      = extd_offset << 2;

  bool cond = cpu->get_reg(instr.field1) != cpu->get_reg(instr.field2);

  Word pc_prev = cpu->get_pc();
  cpu->set_pc(cond ? pc_prev + sizeof(Word) + target
                   : pc_prev + sizeof(Word));
}

void Cpu::execute_beq(Instr instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);
  Word          target      = extd_offset << 2;

  bool cond = cpu->get_reg(instr.field1) == cpu->get_reg(instr.field2);

  Word pc_prev = cpu->get_pc();
  cpu->set_pc(cond ? pc_prev + sizeof(Word) + target
                   : pc_prev + sizeof(Word));
}

void Cpu::execute_selc(Instr instr) {
  Register rs1  = cpu->get_reg(instr.field2);
  Register rs2  = cpu->get_reg(instr.field3);
  bool     cond = rs1 > rs2;

  Register result = cond ? rs1 : rs2;
  cpu->set_reg(instr.field1, result);

  INCR_PC;
}

void Cpu::execute_sti(Instr instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = cpu->get_reg(instr.field1) + extd_offset;
  memory_.store(addr, cpu->get_reg(instr.field2));
  cpu->set_reg(instr.field1, addr);

  INCR_PC;
}

void Cpu::execute_j(Instr instr) {
  Register pc = cpu->get_pc();
  cpu->set_pc((pc & 0xF0000000) | (instr.field3 << 2));
}

void Cpu::execute_ssat(Instr instr) {
  Register ssatred = sgn_sat(cpu->get_reg(instr.field1), instr.field3);
  cpu->set_reg(instr.field1, ssatred);

  INCR_PC;
}

void Cpu::execute_ld(Instr instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = cpu->get_reg(instr.field1) + extd_offset;
  Word        val  = memory_.load<Word>(addr);
  cpu->set_reg(instr.field2, val);

  INCR_PC;
}

void Cpu::execute_sbit(Instr instr) {
  Register sbit = 1 << instr.field3;
  cpu->set_reg(instr.field1, sbit);

  INCR_PC;
}

void Cpu::execute_add(Instr instr) {
  Register sum = cpu->get_reg(instr.field1) + cpu->get_reg(instr.field3);
  cpu->set_reg(instr.field2, sum);

  INCR_PC;
}

void Cpu::execute_addi(Instr instr) {
  const std::uint32_t bit_width = FIELD3_WIDTH(instr.instr);
  const SignedWord immediate = sgn_extend(instr.field3, bit_width);
  Register sum = cpu->get_reg(instr.field1) + immediate;
  cpu->set_reg(instr.field2, sum);

  INCR_PC;
}

void Cpu::load_instrs(std::vector<Word> &instrs) {
  memory_.load_instrs(0, instrs);
}

Word Cpu::fetch_instr() {
  return memory_.load<Word>(cpu->get_pc());
}

Instr Cpu::decoder(Word instr_code) {
  Instr instr{};
  GET_OPCODE(instr_code, instr);

  switch(instr.opc) {
    case Opcode::opClz  : GET_ALL_FIELDS(kClz );
    case Opcode::opLi   : GET_ALL_FIELDS(kLi  );
    case Opcode::opSysc : GET_ALL_FIELDS(kSysc);
    case Opcode::opSt   : GET_ALL_FIELDS(kSt  );
    case Opcode::opStp  : GET_ALL_FIELDS(kStp );
    case Opcode::opBne  : GET_ALL_FIELDS(kBne );
    case Opcode::opBeq  : GET_ALL_FIELDS(kBeq );
    case Opcode::opSelc : GET_ALL_FIELDS(kSelc);
    case Opcode::opSti  : GET_ALL_FIELDS(kSti );
    case Opcode::opJ    : GET_ALL_FIELDS(kJ   );
    case Opcode::opSsat : GET_ALL_FIELDS(kSsat);
    case Opcode::opLd   : GET_ALL_FIELDS(kLd  );
    case Opcode::opSbit : GET_ALL_FIELDS(kSbit);
    case Opcode::opAdd  : GET_ALL_FIELDS(kAdd );
    case Opcode::opAddi : GET_ALL_FIELDS(kAddi);
  }

  return instr;
}

void BtCdExec::executor(Instr instr) {
  switch(instr.instr) {
    case kClz:
      execute_clz(instr);
      break;
    case kLi:
      execute_li(instr);
      break;
    case kSysc:
      execute_sysc(instr);
      break;
    case kSt:
      execute_st(instr);
      break;
    case kStp:
      execute_stp(instr);
      break;
    case kBne:
      execute_bne(instr);
      break;
    case kBeq:
      execute_beq(instr);
      break;
    case kSelc:
      execute_selc(instr);
      break;
    case kSti:
      execute_sti(instr);
      break;
    case kJ:
      execute_j(instr);
      break;
    case kSsat:
      execute_ssat(instr);
      break;
    case kLd:
      execute_ld(instr);
      break;
    case kSbit: 
      execute_sbit(instr);
      break;
    case kAdd: 
      execute_add(instr);
      break;
    case kAddi: 
      execute_addi(instr);
      break;
  }
}

Register CoreState::get_reg(std::size_t spec) {
  check_range(spec);

  return gpr_regs[spec];
}

Register CoreState::get_pc() {
  return pc;
}

void CoreState::set_reg(std::size_t spec, Register val) {
  check_range(spec);

  gpr_regs[spec] = val;
}

void CoreState::set_pc(Register val) {
  pc = val;
}

void CoreState::check_range(std::size_t spec) {
  if (spec < 0 || spec >= kNumRegs) {
    throw std::out_of_range("cpu: register is out of range");
  }
}

void BtCdExec::execute_block(BasicBlock &blk) {
  for(auto& instr : blk) {
    executor(instr);
  }
}

void ThrdCdExec::execute_block(BasicBlock &blk) {
  
}

ThrdCdExec::Handler ThrdCdExec::handler(InstrNumb instr) {
  switch (instr) {
    case kClz  : return &ThrdCdExec::execute_clz;
    case kLi   : return &ThrdCdExec::execute_li;
    case kSysc : return &ThrdCdExec::execute_sysc;
    case kSt   : return &ThrdCdExec::execute_st;
    case kStp  : return &ThrdCdExec::execute_stp;
    case kBne  : return &ThrdCdExec::execute_bne;
    case kBeq  : return &ThrdCdExec::execute_beq;
    case kSelc : return &ThrdCdExec::execute_selc;
    case kSti  : return &ThrdCdExec::execute_sti;
    case kJ    : return &ThrdCdExec::execute_j;
    case kSsat : return &ThrdCdExec::execute_ssat;
    case kLd   : return &ThrdCdExec::execute_ld;
    case kSbit : return &ThrdCdExec::execute_sbit;
    case kAdd  : return &ThrdCdExec::execute_add;
    case kAddi : return &ThrdCdExec::execute_addi;

  }
}
