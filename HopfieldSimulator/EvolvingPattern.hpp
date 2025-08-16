#ifndef EVOLVING_PATTERN_HPP
#define EVOLVING_PATTERN_HPP
#include<vector>
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
  const std::vector<T>& getPattern() const {
    return pattern_; 
  }


  std::vector<T>& getPattern() {
    return pattern_; 
  }
  const std::vector<float>& getEnergy() const {
    return energy_; 
  }


  std::vector<float>& getEnergy() {
    return energy_; 
  }
void clearEnergy(){
  energy_.clear();}


};

template class EvolvingPattern<int8_t>;
template class EvolvingPattern<int>;
template class EvolvingPattern<std::complex<int>>;
template class EvolvingPattern<std::complex<int8_t>>;

} //namespace EP

#endif  //EVOLVING_PATTERN_HPP