#pragma once

#include <cstdint>

#include "memory.hpp"
#include "isa.hpp"

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
};

struct Cpu {
public:
  Cpu() {
    cpu = new CoreState{};
  }

  ~Cpu() {
    delete cpu;
  }

  Instruction decoder  (Word inst_code);
  void        executor (Instruction instr);

private:
  CoreState* cpu;
};
