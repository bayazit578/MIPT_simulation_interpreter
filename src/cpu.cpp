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
}


#define XCUTE_CLZ {                                                     \
  std::uint32_t value = cpu->gpr_regs[instr.field2];                    \
  std::uint32_t mask  = 1 << (kWordSize - 1);                           \
  for (std::uint8_t i = 0; i < kWordSize || (value & mask) == 0; i++) { \
    value <<= 1;                                                        \
  }                                                                     \
  cpu->gpr_regs[instr.field1] = i;                                      \
}

#define XCUTE_LI {                                           \
  std::uint32_t bit_width = FIELD3_WIDTH                     \
  SignedWord extd_imm = sgd_extend(instr.field3, bit_width); \
  cpu->gpr_regs[instr.field1] = (Word)extd_imm;              \
}

#define XCUTE_SYSC {\
}

#define XCUTE_ST {\
}

#define XCUTE_STP {\
}

#define XCUTE_BNE {\
}

#define XCUTE_BEQ {\
}

#define XCUTE_SELC {\
}

#define XCUTE_STI {\
}

#define XCUTE_J {\
}

#define XCUTE_SSAT {\
}

#define XCUTE_LD {\
}

#define XCUTE_SBIT {\
}

#define XCUTE_ADD {\
}

#define XCUTE_ADDI {\
}


static SignedWord sgn_extend(Word value, Word bit_width) {
  Word m = 1u << (bit_width - 1);
  return std::bit_cast<SignedWord>((value ^ m) - m);
}

static std::uint32_t get_mask(std::uint32_t mask_width) {
  return ((1 << mask_width) - 1);
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
    case kClz  : XCUTE_CLZ;
    case kLi   : XCUTE_LI;
    case kSysc : XCUTE_SYSC;
    case kSt   : XCUTE_ST;
    case kStp  : XCUTE_STP;
    case kBne  : XCUTE_BNE;
    case kBeq  : XCUTE_BEQ;
    case kSelc : XCUTE_SELC;
    case kSti  : XCUTE_STI;
    case kJ    : XCUTE_J;
    case kSsat : XCUTE_SSAT;
    case kLd   : XCUTE_LD;
    case kSbit : XCUTE_SBIT;
    case kAdd  : XCUTE_ADD;
    case kAddi : XCUTE_ADDI;
  }
}
