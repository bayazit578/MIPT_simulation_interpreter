#pragma once

#include <cstdlib>
#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "isa.hpp"

struct Memory {
public:
  Memory()
    : data{}
  {
  }

  template<MemoryType M>
  M load(std::size_t addr) const;

  template<MemoryType M>
  void store(std::size_t addr, M value);

private:
  std::array<std::uint8_t, kMemorySize> data;

  template<MemoryType M>
  void check_range(std::size_t addr) const;
};


template<MemoryType M>
M Memory::load(std::size_t addr) const {
  check_range<M>(addr);

  M value{};

  std::memcpy(&value, &data[addr], sizeof(M));

  return value;
}

template<MemoryType M>
void Memory::store(std::size_t addr, M value) {
  check_range<M>(addr);

  std::memcpy(&data[addr], &value, sizeof(M));
}

template<MemoryType M>
void Memory::check_range(std::size_t addr) const {
  if (addr < 0 || addr >= data.size() || sizeof(M) > data.size() - addr) {
    throw std::runtime_error("Memory address out of range\n");
  }
}
