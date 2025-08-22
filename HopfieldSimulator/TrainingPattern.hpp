#ifndef TRAINING_PATTERN_HPP
#define TRAINING_PATTERN_HPP

#include <memory>
#include <random>
#include <string>
#include <vector>


#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>


namespace TP{



struct StbiImageDeleter {
    void operator()(unsigned char* data) const;
};


inline std::mt19937& getRandomGenerator(uint32_t seed_value = std::random_device{}()) {
    static std::mt19937 gen(seed_value); 
    return gen;
}

template <typename T = int8_t> 

class TrainingPattern {
private:
// variables
   static constexpr
   std::array<T,MD::getMathematicalNumberVertex<T>()>POINTS =
    MD::getMathematicalVertex<T>();
  std::vector<T> pattern_;
  std::unique_ptr<unsigned char[], StbiImageDeleter> imgData_;
    int imgWidth_ = 0;
    int imgHeight_ = 0;
    int imgChannels_ = 0;
    std::vector<long long> imgIntegral_; 
    float randomDensity_;
  void generateRandomPattern(const float randomDensity, const std::size_t numColumns, const std::size_t numRows);
    void calculateIntegralImage();

public:
  TrainingPattern(const std::string& path, const std::size_t numColumns, const std::size_t numRows);
  TrainingPattern(const float randomDensity, const std::size_t numColumns, const std::size_t numRows);
  TrainingPattern() = delete;
  const std::vector<T> &getPattern() const noexcept;
  void regrid(const std::size_t numColumns, const std::size_t numRows);
};

template class TrainingPattern<int8_t>;
template class TrainingPattern<int>;
template class TrainingPattern<std::complex<int>>;
template class TrainingPattern<std::complex<int8_t>>;


} //namespace TP

#endif  //TRAINING_PATTERN_HPP