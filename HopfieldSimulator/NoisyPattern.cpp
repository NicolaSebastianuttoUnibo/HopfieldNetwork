#include "NoisyPattern.hpp"

#include "math/RandomUtils.hpp"

 #include <stdexcept> 
#include <algorithm> 
#include <iterator>
//private functions
//generateCorruptedPattern()
template <typename T> 
void NP::NoisyPattern<T>::generateCorruptedPattern(const float noise) {
if (noise < 0.0f || noise > 1.0f) {
        throw std::invalid_argument("The noise must be in the interval [0.0 , 1.0]");
    }


std::uniform_real_distribution<float> real(0.0f, 1.0f);
std::uniform_int_distribution<int> integer(0, static_cast<int>(POINTS.size() - 1));
auto& generator = RU::getRandomGenerator();

     for (auto &val : pattern_) {
          if (real(generator) < noise) {
             int random_index = integer(generator);
             val = POINTS[random_index];
     }


}
}
///public functions

//constructor
template <typename T> 
NP::NoisyPattern<T>::NoisyPattern(const std::vector<T> &sourcePattern, const float noise) : pattern_(sourcePattern)  {
 
  generateCorruptedPattern(noise);
}


//getPattern()
template <typename T> 
const std::vector<T>& NP::NoisyPattern<T>::getPattern() const noexcept{
  return pattern_;
}

//cyclePixelState()
template <typename T> 
void NP::NoisyPattern<T>::cyclePixelState(const std::size_t pixelIndex) {
if (pixelIndex>=pattern_.size()) {
throw std::invalid_argument("pixelIndex is out of bounds.");
}
 auto it = std::find(POINTS.begin(), POINTS.end(), pattern_[pixelIndex]);

  
    if (it == POINTS.end()) {
        throw std::invalid_argument("Current pixel value is not found in POINTS set.");
    }
    pattern_[pixelIndex] = POINTS[(std::distance(POINTS.begin(), it) + 1) % POINTS.size()];



}
