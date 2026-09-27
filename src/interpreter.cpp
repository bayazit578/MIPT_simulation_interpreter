#include "interpreter.hpp"

int Interpreter::load_program(std::string fname) {
  std::ifstream file{fname, std::ios::binary | std::ios::ate};
  if (!file.is_open()) {
    std::cerr << "erroe with file opening" << std::endl;
  }

  const std::streamoff fsize = file.tellg();
  std::vector<Word> instrs {static_cast<std::uint32_t>(fsize)};

  file.seekg(std::ios::beg);
  if(!file.read(reinterpret_cast<char*>(instrs.data()), fsize)) {
    std::cerr << "interpreter: could not read file" << std::endl;
    return EXIT_FAILURE;
  }

  cpu_.load_instrs(instrs);

  return EXIT_SUCCESS;
}

void Interpreter::inter() {
  Cpu cpu;
  Word instr_code   = cpu.fetch_instr();
  Instruction instr = cpu.decoder(instr_code);
  cpu.executor(instr);
}
