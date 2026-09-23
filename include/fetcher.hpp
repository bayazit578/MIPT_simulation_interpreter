#pragma once

#include <fstream>
#include <string>

#include "isa.hpp"

class Fetcher {
public:
  std::ifstream input;

  Fetcher(std::string fname)
    : input{fname, std::ios::in}
  {
  }

  Word load_instr();
  bool is_eof();
};
