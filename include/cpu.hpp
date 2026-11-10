#pragma once

#include <cstdint>
#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <stdexcept>

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

  static std::size_t reg_offset(std::size_t index);
  static std::size_t pc_offset ();

private:
  Register gpr_regs[kNumRegs];
  Register pc;

  void check_range(std::size_t spec);
};


class Executor {
public:
  Executor()
    : memory_{}
  {
    cpu_ = new CoreState{};
  }

  virtual ~Executor() {
    delete cpu_;
  }

  Executor(const Executor&) = delete;
  Executor& operator=(const Executor&) = delete;

  using BasicBlock = std::vector<Instr>;

  virtual void reset();
  virtual void execute_next_block() = 0;
  BasicBlock   decode_next_block();
  
  Register pc () {
    return cpu_->get_pc();
  }

  Register reg(std::size_t spec) {
    return cpu_->get_reg(spec);
  }

  void set_syscall_result(Register value) {
    cpu_->set_reg(0, value);
  }

  void  load_instrs(std::vector<Word> &instrs);
  Word  fetch_instr(size_t address);
  Instr decoder    (Word inst_code);

  struct Syscall {
    Register num;
    std::array<Register, kSyscArgNum> args;
  };

  Byte* guest_ptr(std::size_t addr, std::size_t count) {
    return memory_.ptr(addr, count);
  }

protected:
  CoreState *cpu_;
  Memory    memory_;

  std::unordered_map<Register, BasicBlock> cache_;

private:
  std::size_t program_bytes_ = 0;
};


class BtCdExec : public Executor {
public:
  void reset() override;
  void execute_next_block() override;

protected:
  void executor (Instr instr);

private:
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
  void execute_add (Instr instr);
  void execute_addi(Instr instr);
};


class ThrdExec : public Executor {
public:
  void execute_next_block() override;

protected:
  using Handler = void (ThrdExec::*)(Instr, const Instr*, const Instr*);

  void execute_clz (Instr instr, const Instr *next, const Instr *end);
  void execute_li  (Instr instr, const Instr *next, const Instr *end);
  void execute_sysc(Instr instr, const Instr *next, const Instr *end);
  void execute_st  (Instr instr, const Instr *next, const Instr *end);
  void execute_stp (Instr instr, const Instr *next, const Instr *end);
  void execute_bne (Instr instr, const Instr *next, const Instr *end);
  void execute_beq (Instr instr, const Instr *next, const Instr *end);
  void execute_selc(Instr instr, const Instr *next, const Instr *end);
  void execute_sti (Instr instr, const Instr *next, const Instr *end);
  void execute_j   (Instr instr, const Instr *next, const Instr *end);
  void execute_ssat(Instr instr, const Instr *next, const Instr *end);
  void execute_ld  (Instr instr, const Instr *next, const Instr *end);
  void execute_sbit(Instr instr, const Instr *next, const Instr *end);
  void execute_add (Instr instr, const Instr *next, const Instr *end);
  void execute_addi(Instr instr, const Instr *next, const Instr *end);

private:
  static constexpr std::array<Handler, 15> handlers {
    &ThrdExec::execute_clz,
    &ThrdExec::execute_li ,
    &ThrdExec::execute_sysc,
    &ThrdExec::execute_st,
    &ThrdExec::execute_stp,
    &ThrdExec::execute_bne,
    &ThrdExec::execute_beq,
    &ThrdExec::execute_selc,
    &ThrdExec::execute_sti,
    &ThrdExec::execute_j,
    &ThrdExec::execute_ssat,
    &ThrdExec::execute_ld,
    &ThrdExec::execute_sbit,
    &ThrdExec::execute_add,
    &ThrdExec::execute_addi
  };
};
