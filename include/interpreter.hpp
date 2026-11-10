#pragma once

#include <string>
#include <utility>

#include "cpu.hpp"
#include "jit_executor.hpp"

class Interpreter {
public:
  enum class Mode {Bytecode, Threaded, Jit};

  static Executor* select_executor(Mode      mode, 
                                   Executor &bytecode,
                                   Executor &threaded, 
                                   Executor &jit) {
    switch (mode) {
      case Mode::Bytecode:
        return &bytecode;
      case Mode::Threaded:
        return &threaded;
      case Mode::Jit:
        return &jit;
    }
  }

  Interpreter(Mode mode = Mode::Bytecode)
    : cpu_{select_executor(mode, bytecode_, threaded_, jit_)},
      terminated_{false},
      exit_code_{0}
  {
  }

  

  int load_program(const std::string fname);
  void iter();

  bool terminated() const {
    return terminated_;
  }

  int exit_code() const {
    return exit_code_;
  }

private:
  BtCdExec    bytecode_;
  ThrdExec    threaded_;
  JitExec     jit_;
  Executor*   cpu_;
  bool        terminated_;
  int         exit_code_;
};
