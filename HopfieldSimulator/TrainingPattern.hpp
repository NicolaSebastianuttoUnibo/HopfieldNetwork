#ifndef TRAINING_PATTERN_HPP
#define TRAINING_PATTERN_HPP
#include <string>
#include <vector>
#include <cstdint>



namespace TP{
template <typename T = int8_t> 


class TrainingPattern {
  std::string path_;
  std::vector<T> pattern_;

public:
  TrainingPattern(const std::string& path, const std::size_t numColumns, const std::size_t numRows);
  TrainingPattern() = delete;
  const std::vector<T> &getPattern() const;
  void regrid(const std::size_t numColumns, const std::size_t numRows);
};

template class TrainingPattern<int8_t>;
template class TrainingPattern<int>;


} //namespace TP

#endif  //TRAINING_PATTERN_HPP