#ifndef EVOLVING_PATTERN_HPP
#define EVOLVING_PATTERN_HPP
#include<vector>
#include <cstdint>

namespace EP {


class EvolvingPattern {

  std::vector<int8_t> pattern_;

public:
  EvolvingPattern(const std::vector<int8_t> &pattern);
  EvolvingPattern() = delete;
  const std::vector<int8_t> &getPattern() const;
  void  updatePattern(const std::vector<int8_t> &newPattern);

};

} //namespace EP

#endif  //EVOLVING_PATTERN_HPP