#include <iostream>

#include "interpreter.hpp"

int main() {
  Interpreter intrprtr;
  intrprtr.load_program("huihuihui");
  intrprtr.inter();

  return EXIT_SUCCESS;
}
