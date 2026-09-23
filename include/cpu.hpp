#pragma once

#include <cstdint>

#include "memory.hpp"
#include "isa.hpp"

#include "decoder.hpp"

using Register = std::uint32_t;

struct CoreState {
public:
  Register gpr_regs[kNumRegs];
  Register pc;
  Memory*  memory;

  CoreState()
    : gpr_regs{}, pc{0}, memory{} 
  {
  };

  ~CoreState();

  Instruction decoder(Word inst_code);
  void        executor();

private:
  Byte get_opcode_type1(Word inst_code);
  Byte get_opcode_type2(Word inst_code);
};

struct Cpu {
public:
  Cpu() {
    cpu = new CoreState{};
  }

  ~Cpu() {
    delete cpu;
  }

private:
  CoreState* cpu;
};
