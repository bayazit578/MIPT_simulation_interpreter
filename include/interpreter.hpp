#pragma once

#include <string>

#include "cpu.hpp"

class Interpreter {
public:
  enum class Mode {Bytecode, Threaded};

  Interpreter(Mode mode = Mode::Bytecode)
    : cpu_{
          mode == Mode::Bytecode
        ? static_cast<Cpu*>(&bytecode_)
        : static_cast<Cpu*>(&threaded_)
      },
      terminated_{false},
      exit_code_{0}
  {
  }

  int load_program(std::string fname);
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
  Cpu*        cpu_;
  bool        terminated_;
  int         exit_code_;
};
