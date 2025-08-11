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



class NoisyPattern {

  std::vector<int8_t> pattern_;
    void generateCorruptedPattern(const float noise=0.1f);
public:
  NoisyPattern(const std::vector<int8_t> &sourcePattern, const float noise=0.1f);
  NoisyPattern() = delete;
  void flipPixel(const std::size_t pixelIndex);
  const std::vector<int8_t> &getPattern() const noexcept;
};

} //namespace NP

#endif  //NOISY_PATTERN_HPP