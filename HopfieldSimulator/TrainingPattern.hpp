#ifndef TRAINING_PATTERN_HPP
#define TRAINING_PATTERN_HPP
#include <string>
#include <vector>
#include <cstdint>

namespace TP{

class TrainingPattern {
  std::string path_;
  std::vector<int8_t> pattern_;

public:
  TrainingPattern(const std::string& path, const std::size_t numColumns, const std::size_t numRows);
  TrainingPattern() = delete;
  const std::vector<int8_t> &getPattern() const;
  void regrid(const std::size_t numColumns, const std::size_t numRows);
};

} //namespace TP

#endif  //TRAINING_PATTERN_HPP