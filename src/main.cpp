#include <cxxopts.hpp>

#include "cpu.hpp"
#include "interpreter.hpp"

int main(const int argc, const char *const *argv) {
  cxxopts::Options options("Interpreter", "TOY ISA Interpreter");

  options.add_options()
    (
      "i, input",
      "Input binary file",
      cxxopts::value<std::string>()
    )(
      "m, mode",
      "Execution mode: b (bytecode), t (threaded code). Default value: b",
      cxxopts::value<char>()->default_value("b")
    )(
      "h, help",
      "Print help"
    );

  try {
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help();
      return EXIT_SUCCESS;
    }

    if (!result.count("input")) {
      std::cerr << "No input file\n";
      return EXIT_FAILURE;
    }

    std::string in_file = result["input"].as<std::string>();
    char        mode    = result["mode" ].as<char>       ();

    Interpreter::Mode execution_mode;
    switch (mode) {
      case 'b': execution_mode = Interpreter::Mode::Bytecode; break;
      case 't': execution_mode = Interpreter::Mode::Threaded; break;
      default:
        std::cerr << "Invalid execution mode: " << mode << '\n';
        return EXIT_FAILURE;
    }

    Interpreter interpreter{execution_mode};
    if (interpreter.load_program(in_file) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    while (!interpreter.terminated()) {
      interpreter.iter();
    }
  } catch (const cxxopts::exceptions::exception& e) {
    std::cerr << "Argument error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
