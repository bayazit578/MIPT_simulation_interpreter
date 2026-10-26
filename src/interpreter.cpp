#include "interpreter.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <unistd.h>

#include "elf_ext.hpp"

int Interpreter::load_program(const std::string fname) {
  try {
    ElfReader reader;
    reader.load_program(fname);
    auto segments = reader.load_segments();

    if (reader.entry() != 0 || segments.size() != 1) {
      throw std::runtime_error("unsupported ELF layout");
    }

    const Segment& text = segments.front();
    if (text.addr != 0 ||
        !(text.flags & ELFIO::PF_X) ||
        text.data.empty() ||
        text.data.size() % sizeof(Word) != 0 ||
        text.memory_size > kMemorySize) {
      throw std::runtime_error("invalid text segment");
    }

    std::vector<Word> instructions(text.data.size() / sizeof(Word));
    std::memcpy(instructions.data(), text.data.data(), text.data.size());

    cpu_->reset();
    cpu_->load_instrs(instructions);
    terminated_ = false;
    exit_code_ = 0;
    return EXIT_SUCCESS;
  } catch (const std::exception& error) {
    std::cerr << "interpreter: " << error.what() << '\n';
    return EXIT_FAILURE;
  }
}void Interpreter::iter() {
  try {
    cpu_->execute_next_block();
  }

  catch (const Cpu::Syscall &sysc) {
    switch (sysc.num) {
      case kExit : 
        exit_code_  = sysc.args[0];
        terminated_ = true;
        break;
      case kWrite: {
        auto* buf = cpu_->guest_ptr(sysc.args[1], sysc.args[2]);
        ::write(sysc.args[0], buf, sysc.args[2]);
        break;
      }
      case kRead: {
        auto* buf = cpu_->guest_ptr(sysc.args[1], sysc.args[2]);
        ::read(sysc.args[0], buf, sysc.args[2]);
        break;
      }
      case kReadInt: {
        Register value = 0;
        if (!(std::cin >> value)) {
          throw std::runtime_error("interpreter: could not read integer");
        }
        cpu_->set_syscall_result(value);
        break;
      }
      case kWriteInt:
        std::cout << sysc.args[0] << std::endl;
        break;
      default: 
        throw std::runtime_error("interpreter: unknown syscall");
    }
  }
}
