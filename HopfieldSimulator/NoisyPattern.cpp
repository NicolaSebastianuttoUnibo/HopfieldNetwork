#include "NoisyPattern.hpp"
#include <vector>
 #include <stdexcept> 
#include <random>
#include <cstdint>

//constructors
NP::NoisyPattern::NoisyPattern(const std::vector<int8_t> &sourcePattern, const float noise) : pattern_(sourcePattern)  {
  generateCorruptedPattern(noise);
}

void NP::NoisyPattern::generateCorruptedPattern(const float noise) {
if (noise < 0.0f || noise > 1.0f) {
        throw std::invalid_argument("The noise must be in the interval [0.0 , 1.0]");
    }


std::uniform_real_distribution<float> dist(0.0f, 1.0f);
auto& generator = getRandomGenerator();

     for (auto &val : pattern_) {
          if (dist(generator) < noise) {
             val = -val;
     }


}
}

const std::vector<int8_t>& NP::NoisyPattern::getPattern() const noexcept{
  return pattern_;
}

void NP::NoisyPattern::flipPixel(const std::size_t pixelIndex) {
if (pixelIndex>=pattern_.size()) {
throw std::invalid_argument("pixelIndex is out of bounds.");
}


      pattern_[pixelIndex] *= -1;

}
