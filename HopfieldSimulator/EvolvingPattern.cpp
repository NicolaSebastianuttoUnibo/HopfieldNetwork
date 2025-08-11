#include "EvolvingPattern.hpp"
#include <vector>
#include <cstdint>

//constructor
EP::EvolvingPattern::EvolvingPattern(const std::vector<int8_t> &pattern) : pattern_(pattern)  {}

  void  EP::EvolvingPattern::updatePattern(const std::vector<int8_t> &newPattern){
  pattern_=newPattern;
  }


const std::vector<int8_t>& EP::EvolvingPattern::getPattern() const{
  return pattern_;
}

