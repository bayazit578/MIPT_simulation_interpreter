#pragma once

#include <cstdlib>
#include <array>
#include <cstdint>

#include "isa.hpp"

using Byte = std::uint8_t;
using Word = std::uint16_t;

template <MemoryType M>
struct Memory {
public:
  Memory(size_t sz)
    : data{}
  {
  };

  M load(std::size_t addr) const;

  void store(std::size_t addr, M value);

private:
  std::array<std::uint8_t, kMemorySize> data;

  void check_range();
};
