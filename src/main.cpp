#include <iostream>

#include "cpu.hpp"
#include "interpreter.hpp"

int main(const int argc, char *argv[]) {
  Interpreter interpreter;
  if (interpreter.load_program("test_program.bin") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  for (int i = 0; i < 3; ++i) {
    interpreter.iter();
  }
}
