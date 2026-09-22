#pragma once

#include <cstdint>

#include "memory.hpp"

using Register = std::uint32_t;

constexpr std::size_t kNumRegs = 32;

struct CoreState {
  Register gpr_regs[kNumRegs];
  Register pc;
  Memory*  memory;   
};
