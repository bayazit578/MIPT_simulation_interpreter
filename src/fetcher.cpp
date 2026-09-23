#include "fetcher.hpp"

Word Fetcher::load_instr() {
  Word inst_buf{0};

  input.read(
      reinterpret_cast<char*>(inst_buf),
      sizeof(Word)
  );

  return inst_buf;
}

bool Fetcher::is_eof() {
  return input.eof();
}
