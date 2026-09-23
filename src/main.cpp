#include <iostream>

#include "memory.hpp"

int main() {
  Memory memory;
  uint8_t value = memory.load<uint8_t>(1025);
  std::cout << value << "\n";

  return 0;
}
