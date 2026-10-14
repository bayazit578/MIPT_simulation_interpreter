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
  
  Register pc () {
    return cpu->get_pc();
  }

  Register reg(std::size_t spec) {
    return cpu->get_reg(spec);
  }

  void  load_instrs(std::vector<Word> &instrs);
  Word  fetch_instr();
  Instr decoder    (Word inst_code);

  using BasicBlock = std::vector<Instr>;

  virtual void     execute_block       (BasicBlock &blk);
  virtual Register prefetch_basic_block(BasicBlock &blk);

protected:
  struct Syscall {
    Register num;
    std::array<Register, kSyscArgNum> args;
  };

  using Handler = void (Cpu::*) (Instr);

  void execute_clz (Instr instr);
  void execute_li  (Instr instr);
  void execute_sysc(Instr instr);
  void execute_st  (Instr instr);
  void execute_stp (Instr instr);
  void execute_bne (Instr instr);
  void execute_beq (Instr instr);
  void execute_selc(Instr instr);
  void execute_sti (Instr instr);
  void execute_j   (Instr instr);
  void execute_ssat(Instr instr);
  void execute_ld  (Instr instr);
  void execute_sbit(Instr instr);
  void execute_add (Instr isntr);
  void execute_addi(Instr instr);

private:
  CoreState *cpu;
  Memory    memory_;
};


class BtCdExec : public Cpu {
public:
  void execute_block (BasicBlock &blk) override;

private:
  void executor (Instr instr);
};


class ThrdCdExec : public Cpu {
public:

  struct ThrdCdCell {
  public:

    InstrNumb instr;
    Handler   handler;
  };

  void execute_block (BasicBlock &blk) override;

private:
  Handler handler(InstrNumb instr);
};
