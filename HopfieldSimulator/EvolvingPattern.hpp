#ifndef EVOLVING_PATTERN_HPP
#define EVOLVING_PATTERN_HPP

#include <vector>

#include "math/MathDimension.hpp"
#include <complex>
#include <cstdint>

namespace EP {

template <typename T = int8_t> class EvolvingPattern {

  std::vector<T> pattern_;
  std::vector<float> energy_;

public:
  EvolvingPattern(const std::vector<T> &pattern);
  EvolvingPattern() = delete;
  const std::vector<T> &getPattern() const noexcept;
  std::vector<T> &getPattern() noexcept;
  const std::vector<float> &getEnergy() const noexcept;
  std::vector<float> &getEnergy() noexcept;
  void clearEnergy() noexcept;
};

template class EvolvingPattern<signed char>;
template class EvolvingPattern<int>;
template class EvolvingPattern<std::complex<int>>;
template class EvolvingPattern<std::complex<int8_t>>;

} // namespace EP

#endif // EVOLVING_PATTERN_HPP