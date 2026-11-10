#pragma once

#include <stdexcept>

#include "isa.hpp"

#define INCR_PTR data.cc.add  (data.pc_val, sizeof(Word))

#define FIELD1_OFFSET(instr_numb) oper_info[instr_numb].field1.offset
#define FIELD2_OFFSET(instr_numb) oper_info[instr_numb].field2.offset
#define FIELD3_OFFSET(instr_numb) oper_info[instr_numb].field3.offset
#define FIELD4_OFFSET(instr_numb) oper_info[instr_numb].field4.offset
#define FIELD1_WIDTH(instr_numb)  oper_info[instr_numb].field1.width
#define FIELD2_WIDTH(instr_numb)  oper_info[instr_numb].field2.width
#define FIELD3_WIDTH(instr_numb)  oper_info[instr_numb].field3.width
#define FIELD4_WIDTH(instr_numb)  oper_info[instr_numb].field4.width

Word          sgn_extend(Word value, Word bit_width);
std::uint32_t get_mask  (std::uint32_t mask_width);
Word          sgn_sat   (Word reg, Word width);
