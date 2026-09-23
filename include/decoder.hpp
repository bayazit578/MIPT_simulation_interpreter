#pragma once

#include <cstdint>

#include "isa.hpp"

enum class Opcode : std::uint8_t {
  kClz  = 0b100011,
  kLi   = 0b111101,
  kSysc = 0b000111,
  kSt   = 0b001010,
  kStp  = 0b011110,
  kBne  = 0b001011,
  kBeq  = 0b110110,
  kSelc = 0b101001,
  kSti  = 0b101001,
  kJ    = 0b000110,
  kSsat = 0b101011,
  kLd   = 0b001001,
  kSbit = 0b000100,
  kAdd  = 0b000011,
  kAddi = 0b011100
};

struct Instruction {
public:
  Opcode opc{};

  Byte hui1{}, hui2{};
  Word hui3{};

  void get_instruction();
};
