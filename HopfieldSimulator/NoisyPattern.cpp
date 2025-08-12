#include "NoisyPattern.hpp"
#include <vector>
 #include <stdexcept> 
#include <random>
#include <cstdint>
#include <cassert>

//constructors
template <typename T> 
NP::NoisyPattern<T>::NoisyPattern(const std::vector<T> &sourcePattern, const float noise) : pattern_(sourcePattern)  {
 
  generateCorruptedPattern(noise);
}

template <typename T> 
void NP::NoisyPattern<T>::generateCorruptedPattern(const float noise) {
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

template <typename T> 
const std::vector<T>& NP::NoisyPattern<T>::getPattern() const noexcept{
  return pattern_;
}


template <typename T> 
void NP::NoisyPattern<T>::flipPixel(const std::size_t pixelIndex) {
if (pixelIndex>=pattern_.size()) {
throw std::invalid_argument("pixelIndex is out of bounds.");
}


      pattern_[pixelIndex] *= -1;

}
