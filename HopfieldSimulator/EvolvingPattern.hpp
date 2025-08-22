#ifndef EVOLVING_PATTERN_HPP
#define EVOLVING_PATTERN_HPP

#include<vector>

#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>

namespace EP {


template <typename T = int8_t> 
class EvolvingPattern {

  std::vector<T> pattern_;
  std::vector<float> energy_;


public:
  EvolvingPattern(const std::vector<T> &pattern);
  EvolvingPattern() = delete;
  const std::vector<T>& getPattern() const;
  std::vector<T>& getPattern();
  const std::vector<float>& getEnergy() const;
  std::vector<float>& getEnergy();
void clearEnergy();


};

template class EvolvingPattern<signed char>;
template class EvolvingPattern<int>;
template class EvolvingPattern<std::complex<int>>;
template class EvolvingPattern<std::complex<int8_t>>;

} //namespace EP

#endif  //EVOLVING_PATTERN_HPP