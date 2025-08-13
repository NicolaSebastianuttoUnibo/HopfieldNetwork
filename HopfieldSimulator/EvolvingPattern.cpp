#include "EvolvingPattern.hpp"
#include <vector>
#include <cstdint>



//constructor
template <typename T> 
EP::EvolvingPattern<T>::EvolvingPattern(const std::vector<T> &pattern) : pattern_(pattern)  {}

