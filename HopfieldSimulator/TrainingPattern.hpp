#ifndef TRAINING_PATTERN_HPP
#define TRAINING_PATTERN_HPP
#include <string>
#include <vector>
#include <cstdint>
#include <memory>



namespace TP{


struct StbiImageDeleter {
    void operator()(unsigned char* data) const;
};

template <typename T = int8_t> 


class TrainingPattern {
  std::string path_;
  std::vector<T> pattern_;


  std::unique_ptr<unsigned char[], StbiImageDeleter> imageData_;
    int imgWidth_ = 0;
    int imgHeight_ = 0;
    int imgChannels_ = 0;

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