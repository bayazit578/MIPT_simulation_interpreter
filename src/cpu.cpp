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

#define GET_IMM(instr_code, instr)\ // доделать
  Word imm{0};                    \
  

static std::uint32_t get_mask(std::uint32_t mask_width) {
  return ((1 << mask_width) - 1);
}

Instruction CoreState::decoder(Word instr_code) {
  Instruction instr;
  GET_OPCODE(instr_code, instr);

  switch(instr.opc) {
    kClz  :
    kLi   :
    kSysc :
    kSt   :
    kStp  :
    kBne  :
    kBeq  :
    kSelc :
    kSti  :
    kJ    :
    kSsat :
    kLd   :
    kSbit :
    kAdd  :
    kAddi :
  }

  return instr;
}
