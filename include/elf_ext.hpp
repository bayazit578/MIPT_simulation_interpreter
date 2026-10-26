#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <elfio/elfio.hpp>

#include "isa.hpp"

const uint64_t kPageSize = sysconf(_SC_PAGESIZE);

struct Segment {
  std::size_t       addr;
  std::uint64_t     memory_size;
  std::uint32_t     flags;
  std::vector<Byte> data;
};

class ElfWriter {
public:
  ElfWriter();
  
  void add_text(const std::string& code, std::size_t addr);

  void save(const std::string& fname);

private:
  ELFIO::elfio elf_;
};

class ElfReader {
public:
  void load_program(const std::string fname);

  using SegmentTable = std::vector<Segment>;
  SegmentTable load_segments() const;

  std::size_t entry() {
    return elf_.get_entry();
  }

private:
  ELFIO::elfio elf_;
};
