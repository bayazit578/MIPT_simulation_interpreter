#include "interpreter.hpp"

int Interpreter::load_program(std::string fname) {
  std::ifstream file(fname, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "interpreter: could not open file\n";
    return EXIT_FAILURE;
  }

  const std::streamoff byte_count = file.tellg();
  if (byte_count < 0 
   || byte_count > kMemorySize
   || byte_count % sizeof(Word) != 0) {
    std::cerr << "interpreter: invalid program size\n";
    return EXIT_FAILURE;
  }

  const std::size_t word_count =
      static_cast<std::size_t>(byte_count) / sizeof(Word);
  std::vector<Word> instr_buf(word_count);

  if (!file.seekg(0, std::ios::beg)) {
    std::cerr << "interpreter: could not seek to file start\n";
    return EXIT_FAILURE;
  }

  if (byte_count > 0 &&
      !file.read(reinterpret_cast<char*>(instr_buf.data()),
                 static_cast<std::streamsize>(byte_count))) {
    std::cerr << "interpreter: could not read file\n";
    return EXIT_FAILURE;
  }

  cpu_.load_instrs(instr_buf);
  return EXIT_SUCCESS;
}

void Interpreter::iter() {
  try {
    auto cache_it = cache_.find(cpu_.pc());

    if (cache_it != cache_.end()) {
        cpu_.execute_block(cache_it->second);
        return;
    }

    Cpu::BasicBlock blk;

    Register begin_pc = cpu_.prefetch_basic_block(blk);
    cache_[begin_pc] = blk;
    cpu_.execute_block(blk);
  }

  catch (const Syscall &sysc) {
    switch (sysc.num) {
      case kExit : 
        exit_code_  = sysc.args[0];
        terminated_ = true;
        break;
      default: 
        throw std::runtime_error("interpreter: unknown syscall");
    }
  }
}
