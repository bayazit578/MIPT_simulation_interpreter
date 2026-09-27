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
  instr.field3 &= field4 << FIELD3_WIDTH(instr_numb);                     \
  break;                                                                  \
}

#define INCR_PC set_pc(get_pc() + sizeof(Word))
  

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


void Cpu::execute_clz(Instruction instr) {
  std::uint32_t value = get_reg(instr.field2);
  std::uint32_t mask  = 1 << (kWordSize - 1);
  std::uint32_t i     = 0;

  for (; i < kWordSize || (value & mask) == 0; i++) {
    value <<= 1;
  }

  set_reg(instr.field1, i);

  INCR_PC;
}

void Cpu::execute_li(Instruction instr) {
  std::uint32_t bit_width = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_imm  = sgn_extend(instr.field3, bit_width);
  set_reg(instr.field1, (Register)extd_imm);

  INCR_PC;
}

void Cpu::execute_sysc(Instruction instr) {
}

void Cpu::execute_st(Instruction instr) {
  std::uint32_t bit_width = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_imm  = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = get_reg(instr.field1) + extd_imm;
  memory_.store<Word>(addr, get_reg(instr.field2));

  INCR_PC;
}

void Cpu::execute_stp(Instruction instr) {
  std::uint32_t bit_width = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_imm  = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = get_reg(instr.field1) + extd_imm;
  memory_.store<Word>(addr    , get_reg(instr.field2));
  memory_.store<Word>(addr + 4, get_reg(instr.field3));

  INCR_PC;
}

void Cpu::execute_bne(Instruction instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);
  Word          target      = extd_offset << 2;

  bool cond = instr.field1 != instr.field2;

  Word pc_prev = get_pc();
  set_pc(cond ? pc_prev + target : pc_prev + 4);

  INCR_PC;
}

void Cpu::execute_beq(Instruction instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);
  Word          target      = extd_offset << 2;

  bool cond = instr.field1 == instr.field2;

  Word pc_prev = get_pc();
  set_pc(cond ? pc_prev + target : pc_prev + 4);

  INCR_PC;
}

void Cpu::execute_selc(Instruction instr) {
  Register rs1    = get_reg(instr.field2);
  Register rs2    = get_reg(instr.field3);
  bool     cond   = rs1 > rs2;
  Register result = cond ? rs1 : rs2;

  INCR_PC;
}

void Cpu::execute_sti(Instruction instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = get_reg(instr.field1) + extd_offset;
  memory_.store(addr, get_reg(instr.field2));
  set_reg(instr.field1, addr);

  INCR_PC;
}

void Cpu::execute_j(Instruction instr) {
  Register pc = get_pc();
  set_pc((pc & 0xF0000000) | (instr.field3 << 2));
}

void Cpu::execute_ssat(Instruction instr) {
  Register ssatred = sgn_sat(get_reg(instr.field1), instr.field3);
  set_reg(instr.field1, ssatred);

  INCR_PC;
}

void Cpu::execute_ld(Instruction instr) {
  std::uint32_t bit_width   = FIELD3_WIDTH(instr.instr);
  SignedWord    extd_offset = sgn_extend  (instr.field3, bit_width);

  std::size_t addr = get_reg(instr.field1) + extd_offset;
  Word        val  = memory_.load<Word>(addr);
  set_reg(instr.field2, val);

  INCR_PC;
}

void Cpu::execute_sbit(Instruction instr) {
  Register sbit = 1 << instr.field3;
  set_reg(instr.field1, sbit);

  INCR_PC;
}

void Cpu::execute_add(Instruction instr) {
  Register sum = get_reg(instr.field1) + get_reg(instr.field2);
  set_reg(instr.field3, sum);

  INCR_PC;
}

void Cpu::execute_addi(Instruction instr) {
  Register sum = get_reg(instr.field1) + instr.field3;
  set_reg(instr.field2, sum);

  INCR_PC;
}

void Cpu::load_instrs(std::vector<Word> &instrs) {
  memory_.load_instrs(0, instrs);
}

Word Cpu::fetch_instr() {
  return memory_.load<Word>(get_pc());
}

Instruction Cpu::decoder(Word instr_code) {
  Instruction instr;
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

void Cpu::executor(Instruction instr) {
  switch(instr.instr) {
    case kClz: 
      execute_clz (instr);
      break;
    case kLi: 
      execute_li  (instr);
      break;
    case kSysc: 
      execute_sysc(instr);
      break;
    case kSt: 
      execute_st  (instr);
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

Register Cpu::get_reg(std::size_t spec) {
  return cpu->gpr_regs[spec];
}

Register Cpu::get_pc() {
  return cpu->pc;
}

void Cpu::set_reg(std::size_t spec, Register val) {
  cpu->gpr_regs[spec] = val;
}

void Cpu::set_pc(Register val) {
  cpu->pc = val;
}
