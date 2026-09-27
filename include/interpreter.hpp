#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "cpu.hpp"
#include "isa.hpp"

class Interpreter {
public:
  Interpreter()
    : terminated_{false}
  {
  }

  int load_program(std::string fname);
  void inter();

  bool terminated() {
    return terminated_;
  }

private:
  Cpu  cpu_;
  bool terminated_;
};
