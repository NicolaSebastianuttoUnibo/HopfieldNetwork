#ifndef EVOLVING_PATTERN_HPP
#define EVOLVING_PATTERN_HPP
#include<vector>
#include <cstdint>



namespace EP {


template <typename T = int8_t> 
class EvolvingPattern {

  std::vector<T> pattern_;

public:
  EvolvingPattern(const std::vector<T> &pattern);
  EvolvingPattern() = delete;
  const std::vector<T> &getPattern() const;
  void  updatePattern(const std::vector<T> &newPattern);

};

template class EvolvingPattern<int8_t>;
template class EvolvingPattern<int>;


} //namespace EP

#endif  //EVOLVING_PATTERN_HPP