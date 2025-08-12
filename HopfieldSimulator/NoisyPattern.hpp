#ifndef NOISY_PATTERN_HPP
#define NOISY_PATTERN_HPP
#include <vector>
#include <random>
#include <stdexcept> 
#include <cstdint>


namespace NP {


inline  std::mt19937& getRandomGenerator() {
    static std::mt19937 gen(std::random_device{}()); 
    return gen;
}

template <typename T = int8_t> 
class NoisyPattern {

  std::vector<T> pattern_;
    void generateCorruptedPattern(const float noise=1.0f);
public:
  NoisyPattern(const std::vector<T> &sourcePattern, const float noise=1.0f);
  NoisyPattern() = delete;
  void flipPixel(const std::size_t pixelIndex);
  const std::vector<T> &getPattern() const noexcept;
};

template class NoisyPattern<int8_t>;
template class NoisyPattern<int>;


} //namespace NP

#endif  //NOISY_PATTERN_HPP