#pragma once

#include <unordered_map>
#include <asmjit/core.h>
#include <asmjit/x86.h>

#include "cpu.hpp"

constexpr  std::uint32_t kThreshold = 10;

class JitExec : public BtCdExec {
public:
  JitExec() = default;

  void execute_next_block() override;
  void reset() override;

private:
  asmjit::JitRuntime runtime_;

  struct BasicBlockTemp {
    BasicBlock    instructions;
    std::uint32_t exec_cnt;
  };

  enum BlckExitCode : std::uint32_t{
    kSuccess,
    kSyscall
  };

  using JitFunc = BlckExitCode (*)(CoreState*, Byte*);

  std::unordered_map<Register, BasicBlockTemp> bb_cache_;
  std::unordered_map<Register, JitFunc>        bt_cache_;

  struct JitData {
    asmjit::x86::Compiler& cc;

    asmjit::x86::Gp state;
    asmjit::x86::Gp ram;

    asmjit::x86::Gp tmp1;
    asmjit::x86::Gp tmp2;
    asmjit::x86::Gp addr;

    asmjit::x86::Gp pc_val;
    asmjit::x86::Gp status;
  };

  JitFunc translate(const BasicBlock &block);
  void    interpret(const BasicBlock &block);

  void translate_instr(const Instr &instr, const JitData &data);

  void translate_clz (const Instr &instr, const JitData &data);
  void translate_li  (const Instr &instr, const JitData &data);
  void translate_sysc(const Instr &instr, const JitData &data);
  void translate_st  (const Instr &instr, const JitData &data);
  void translate_stp (const Instr &instr, const JitData &data);
  void translate_bne (const Instr &instr, const JitData &data);
  void translate_beq (const Instr &instr, const JitData &data);
  void translate_selc(const Instr &instr, const JitData &data);
  void translate_sti (const Instr &instr, const JitData &data);
  void translate_j   (const Instr &instr, const JitData &data);
  void translate_ssat(const Instr &instr, const JitData &data);
  void translate_ld  (const Instr &instr, const JitData &data);
  void translate_sbit(const Instr &instr, const JitData &data);
  void translate_add (const Instr &instr, const JitData &data);
  void translate_addi(const Instr &instr, const JitData &data);

  asmjit::x86::Mem reg_to_dword_ptr(const asmjit::x86::Gp& state,
                                    std::size_t index);
  asmjit::x86::Mem pc_to_dword_ptr (const asmjit::x86::Gp& state);
};
