#include "EvolvingPattern.hpp"
#include <vector>
#include <cstdint>


///public functions

//constructor
template <typename T> 
EP::EvolvingPattern<T>::EvolvingPattern(const std::vector<T> &pattern) : pattern_(pattern)  {}

///getPattern() const
template <typename T> 

  const std::vector<T>& EP::EvolvingPattern<T>::getPattern() const {
    return pattern_; 
  }
///getPattern()

template <typename T> 

   std::vector<T>& EP::EvolvingPattern<T>::getPattern()  {
    return pattern_; 
  }

///getEnergy() const

template <typename T> 

    const std::vector<float>& EP::EvolvingPattern<T>::getEnergy() const {
    return energy_; 
  }
///getEnergy() 

template <typename T> 

  std::vector<float>& EP::EvolvingPattern<T>::getEnergy() {
    return energy_; 
  }

///clearEnergy()

template <typename T> 

  void EP::EvolvingPattern<T>::clearEnergy(){
  energy_.clear();}