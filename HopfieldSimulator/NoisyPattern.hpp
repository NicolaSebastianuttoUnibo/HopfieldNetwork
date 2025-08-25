#ifndef NOISY_PATTERN_HPP
#define NOISY_PATTERN_HPP

#include <vector>

#include "math/MathDimension.hpp"
#include <complex>
#include <cstdint>

namespace NP {

template <typename T = int8_t> class NoisyPattern {

  std::vector<T> pattern_;
  void generateCorruptedPattern(const float noise = 0.1f);

public:
  NoisyPattern(const std::vector<T> &sourcePattern, const float noise = 0.1f);
  NoisyPattern() = delete;
  const std::vector<T> &getPattern() const noexcept;
  void cyclePixelState(const std::size_t pixelIndex);
};

template class NoisyPattern<int8_t>;
template class NoisyPattern<int>;
template class NoisyPattern<std::complex<int>>;
template class NoisyPattern<std::complex<int8_t>>;

} // namespace NP

#endif // NOISY_PATTERN_HPP