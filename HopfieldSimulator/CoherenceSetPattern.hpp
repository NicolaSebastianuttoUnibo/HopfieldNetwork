#ifndef COHERENCE_SET_PATTERN_HPP
#define COHERENCE_SET_PATTERN_HPP
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include <vector>
#include <string>
 #include <stdexcept> 

namespace CSP {

class CoherenceSetPattern {
  TP::TrainingPattern tp_;
  NP::NoisyPattern np_;
  EP::EvolvingPattern ep_;


public:
  explicit CoherenceSetPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows);

  void regrid(const std::size_t numColumns, const std::size_t numRows);
  void reCorrupt(const float noise=0.1f);
  void flipNoisyPixel(std::size_t pos);
  void updateEvolvingState(const std::vector<int8_t>& newPattern);//function that updates ep_
  const TP::TrainingPattern& getTrainingPattern() const noexcept;
  const NP::NoisyPattern& getNoisyPattern() const noexcept;
  const EP::EvolvingPattern& getEvolvingPattern() const noexcept;

};

} //namespace

#endif //COHERENCE_SET_PATTERN_HPP