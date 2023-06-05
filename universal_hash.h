#ifndef UNIVERSAL_HASH_H
#define UNIVERSAL_HASH_H
#include <random>

class UniversalHash {
  size_t coef_;
  size_t shift_;
  static constexpr size_t kMod = 3000000019;

 public:
  static const size_t kMaxValue = kMod - 1;
  explicit UniversalHash(size_t coef_param = 1, size_t shift_param = 0) : coef_(coef_param), shift_(shift_param) {
  }
  size_t operator()(size_t number) const noexcept {
    return (coef_ * number + shift_) % kMod;
  }
  static UniversalHash GenerateHash(std::mt19937_64& generator) {
    auto coeff_distribution = std::uniform_int_distribution<size_t>(0, kMaxValue);
    const auto coef = coeff_distribution(generator);
    const auto shift = coeff_distribution(generator);
    return UniversalHash(coef, shift);
  }
};
#endif