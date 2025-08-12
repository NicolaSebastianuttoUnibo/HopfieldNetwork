#include "EvolvingPattern.hpp"
#include <vector>
#include <cstdint>



//constructor
template <typename T> 
EP::EvolvingPattern<T>::EvolvingPattern(const std::vector<T> &pattern) : pattern_(pattern)  {}

template <typename T> 
  void  EP::EvolvingPattern<T>::updatePattern(const std::vector<T> &newPattern){
  pattern_=newPattern;
  }

template <typename T> 
const std::vector<T>& EP::EvolvingPattern<T>::getPattern() const{
  return pattern_;
}

