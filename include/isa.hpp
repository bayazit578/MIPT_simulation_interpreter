#pragma once

#include <cstdint>
#include <concepts>

using Byte       = std::uint8_t;
using Word       = std::uint32_t;
using SignedWord = std::int32_t;

template <typename M>
concept MemoryType =
  std::same_as<M, Byte> || std::same_as<M, Word>;

constexpr std::uint32_t kNumRegs    = 32;
constexpr std::uint32_t kWordSize   = 32;
constexpr std::uint32_t kMemorySize = 1024;

enum InstrNumb : std::uint8_t {
  kClz  =  0,
  kLi   =  1,
  kSysc =  2,
  kSt   =  3,
  kStp  =  4,
  kBne  =  5,
  kBeq  =  6,
  kSelc =  7,
  kSti  =  8,
  kJ    =  9,
  kSsat = 10,
  kLd   = 11,
  kSbit = 12,
  kAdd  = 13,
  kAddi = 14
};

enum class Opcode : std::uint8_t {
  opClz  = 0b100011,
  opLi   = 0b111101,
  opSysc = 0b000111,
  opSt   = 0b001010,
  opStp  = 0b011110,
  opBne  = 0b001011,
  opBeq  = 0b110110,
  opSelc = 0b001111,
  opSti  = 0b101001,
  opJ    = 0b000110,
  opSsat = 0b101011,
  opLd   = 0b001001,
  opSbit = 0b000100,
  opAdd  = 0b000011,
  opAddi = 0b011100
};

struct Instruction {
public:
  InstrNumb instr;

  Opcode    opc{};

  Byte      field1{}, field2{};
  Word      field3{};
};


struct InstrFieldInfo {
public:
  struct Operands {
  public:
    std::uint8_t offset;
    std::uint8_t width;
  };

  InstrNumb instr {}; 
  Opcode    opcode{};
  Operands  field1{};
  Operands  field2{};
  Operands  field3{};
  Operands  field4{};
};

constexpr InstrFieldInfo oper_info[] = {
  {kClz , Opcode::opClz , {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x00}, {0x00, 0x00}},
  {kLi  , Opcode::opLi  , {0x10, 0x05}, {0x00, 0x00}, {0x00, 0x10}, {0x00, 0x00}},
  {kSysc, Opcode::opSysc, {0x00, 0x00}, {0x00, 0x00}, {0x06, 0x14}, {0x00, 0x00}},
  {kSt  , Opcode::opSt  , {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x0E}, {0x00, 0x00}},
  {kStp , Opcode::opStp , {0x15, 0x05}, {0x10, 0x05}, {0x0B, 0x05}, {0x00, 0x0B}},
  {kBne , Opcode::opBne , {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x10}, {0x00, 0x00}},
  {kBeq , Opcode::opBeq , {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x10}, {0x00, 0x00}},
  {kSelc, Opcode::opSelc, {0x15, 0x05}, {0x10, 0x05}, {0x0B, 0x05}, {0x00, 0x00}},
  {kSti , Opcode::opSti , {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x0E}, {0x00, 0x00}},
  {kJ   , Opcode::opJ   , {0x00, 0x1A}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}},
  {kSsat, Opcode::opSsat, {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x00}, {0x00, 0x00}},
  {kLd  , Opcode::opLd  , {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x0E}, {0x00, 0x00}},
  {kSbit, Opcode::opSbit, {0x15, 0x05}, {0x10, 0x05}, {0x0B, 0x05}, {0x00, 0x00}},
  {kAdd , Opcode::opAdd , {0x15, 0x05}, {0x10, 0x05}, {0x0B, 0x05}, {0x00, 0x00}},
  {kAddi, Opcode::opAddi, {0x15, 0x05}, {0x10, 0x05}, {0x00, 0x10}, {0x00, 0x00}}
};
