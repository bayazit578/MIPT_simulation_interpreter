#pragma once

#include <cstdint>
#include <concepts>

using Byte = std::uint8_t;
using Word = std::uint16_t;

template <typename M>
concept MemoryType =
  std::same_as<M, Byte> || std::same_as<M, Word>;

constexpr std::uint32_t kNumRegs    = 32;
constexpr std::uint32_t kMemorySize = 1024;
