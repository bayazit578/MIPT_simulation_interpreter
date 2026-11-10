#include "bit_ops.hpp"

Word sgn_extend(Word value, Word bit_width) {
  Word m = 1u << (bit_width - 1);
  return std::bit_cast<Word>((value ^ m) - m);
}

std::uint32_t get_mask(std::uint32_t mask_width) {
  return ((1 << mask_width) - 1);
}

Word sgn_sat(Word value, Word width) {
  if (width == 0 || width > kWordSize) {
    throw std::runtime_error("ssat: invalid width");
  }

  const Word magnitude = Word{1} << (width - 1);
  const Word lower = Word{0} - magnitude;
  const Word upper = magnitude - 1;
  const Word sign_bit = Word{1} << (kWordSize - 1);

  if (value & sign_bit) {
    return value < lower ? lower : value;
  }
  return value > upper ? upper : value;
}
