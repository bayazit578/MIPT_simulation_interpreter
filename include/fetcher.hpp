#pragma once

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>

struct Fetcher {
  std::ifstream input;

  Fetcher(std::string filename, std::ios::openmode flags)
    : input {filename, flags} {
  }

  ~Fetcher();
};
