#ifndef NOISY_PATTERN_HPP
#define NOISY_PATTERN_HPP

#include <random>
#include <vector>

#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>

namespace NP {

inline std::mt19937& getRandomGenerator(uint32_t seed_value = std::random_device{}()) {
    static std::mt19937 gen(seed_value); 
    return gen;
}

template <typename T = int8_t> 
class NoisyPattern {

     static constexpr
   std::array<T,MD::getMathematicalNumberVertex<T>()>POINTS =
    MD::getMathematicalVertex<T>();

  std::vector<T> pattern_;
    void generateCorruptedPattern(const float noise=0.1f);
public:
  NoisyPattern(const std::vector<T> &sourcePattern, const float noise=0.1f);
  NoisyPattern() = delete;
    const std::vector<T> &getPattern() const noexcept;
  void flipPixel(const std::size_t pixelIndex);
};

template class NoisyPattern<int8_t>;
template class NoisyPattern<int>;
template class NoisyPattern<std::complex<int>>;
template class NoisyPattern<std::complex<int8_t>>;

} //namespace NP

#endif  //NOISY_PATTERN_HPP