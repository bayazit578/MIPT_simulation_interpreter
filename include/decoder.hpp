#pragma once

#include <cstdint>

using Word = std::uint32_t;

enum class Opcode : std::uint8_t {
  kClz  = 0b100011,
  kLi   = 0b111101,
  kSysc = 0b000111,
  kSt   = 0b001010,
  kStp  = 0b011110,
  kBne  = 0b001011,
  kBeq  = 0b110110,
  kSelc = 0b101001,

};

struct Instruction {
  Opcode opc{};
  Word   src1{}, src2{}, dst{};
};
