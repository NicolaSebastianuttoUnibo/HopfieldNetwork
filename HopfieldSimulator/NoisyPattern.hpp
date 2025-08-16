#ifndef NOISY_PATTERN_HPP
#define NOISY_PATTERN_HPP
#include <vector>
#include <random>
#include <stdexcept> 
#include <cstdint>
#include <complex>

#include "MathDimension.hpp"


namespace NP {


inline  std::mt19937& getRandomGenerator() {
    static std::mt19937 gen(std::random_device{}()); 
    return gen;
}

template <typename T = int8_t> 
class NoisyPattern {

   static constexpr std::array<T, static_cast<unsigned int>(std::pow(2,MD::getMathematicalDimension<T>()))> POINTS = MD::getMathematicalVertex<T>();


  std::vector<T> pattern_;
    void generateCorruptedPattern(const float noise=0.1f);
public:
  NoisyPattern(const std::vector<T> &sourcePattern, const float noise=0.1f);
  NoisyPattern() = delete;
  void flipPixel(const std::size_t pixelIndex);
  const std::vector<T> &getPattern() const noexcept;
};

template class NoisyPattern<int8_t>;
template class NoisyPattern<int>;
template class NoisyPattern<std::complex<int>>;
template class NoisyPattern<std::complex<int8_t>>;

} //namespace NP

#endif  //NOISY_PATTERN_HPP