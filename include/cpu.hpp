#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <iostream>
#include <queue>

#include "memory.hpp"
#include "isa.hpp"

using Register = std::uint32_t;

#ifdef DEBUG
#define IF_DEBUG(...) __VA_ARGS__
#else
#define IF_DEBUG(...)
#endif

struct CoreState {
public:
  CoreState()
    : gpr_regs{}, pc{0}
  {
  };

  ~CoreState() = default;

  Register get_reg(std::size_t spec);
  Register get_pc ();
  void     set_reg(std::size_t spec, Register val);
  void     set_pc (Register val);

private:
  Register gpr_regs[kNumRegs];
  Register pc;

  void check_range(std::size_t spec);
};


class Cpu {
public:
  Cpu()
    : memory_{}
  {
    cpu = new CoreState{};
  }

  ~Cpu() {
    delete cpu;
  }

  void        load_instrs(std::vector<Word> &instrs);
  Word        fetch_instr();
  Instruction decoder    (Word inst_code);
  void        executor   (Instruction instr);

  Register pc () {
    return cpu->get_pc();
  }

  Register reg(std::size_t spec) {
    return cpu->get_reg(spec);
  }

  using BasicBlock = std::vector<Instruction>;

  void     execute_block       (BasicBlock &blk);
  Register prefetch_basic_block(BasicBlock &blk);

protected:
  CoreState *cpu;

private:
  Memory memory_;

  void execute_clz (Instruction instr);
  void execute_li  (Instruction instr);
  void execute_sysc(Instruction instr);
  void execute_st  (Instruction instr);
  void execute_stp (Instruction instr);
  void execute_bne (Instruction instr);
  void execute_beq (Instruction instr);
  void execute_selc(Instruction instr);
  void execute_sti (Instruction instr);
  void execute_j   (Instruction instr);
  void execute_ssat(Instruction instr);
  void execute_ld  (Instruction instr);
  void execute_sbit(Instruction instr);
  void execute_add (Instruction isntr);
  void execute_addi(Instruction instr);
};

struct Syscall {
  Register num;
  std::array<Register, kSyscArgNum> args;
};
