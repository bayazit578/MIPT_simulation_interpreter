#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "cpu.hpp"
#include "isa.hpp"

class Interpreter {
public:
  Interpreter()
    : terminated_{false}, exit_code_{0}
  {
  }

  int load_program(std::string fname);
  void iter();

  bool terminated() {
    return terminated_;
  }

  int exit_code() {
    return exit_code_;
  }

private:
  using InstrCache = std::unordered_map<Register, Cpu::BasicBlock>;

  InstrCache  cache_;
  Cpu         cpu_;
  bool        terminated_;
  int         exit_code_;
};
