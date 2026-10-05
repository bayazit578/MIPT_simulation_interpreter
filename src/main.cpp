#include "cpu.hpp"
#include "interpreter.hpp"

int main(const int argc, char *argv[]) {
  Interpreter interpreter;
  if (interpreter.load_program("test_program.bin") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  
  Word iters;

  while (!interpreter.terminated()) {
    interpreter.iter();
  }

  return EXIT_SUCCESS;
}
