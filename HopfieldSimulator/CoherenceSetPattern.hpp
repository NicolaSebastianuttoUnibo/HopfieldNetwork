#ifndef COHERENCE_SET_PATTERN_HPP
#define COHERENCE_SET_PATTERN_HPP
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include "HopfieldNetwork.hpp"
#include <vector>
#include <string>
 #include <stdexcept> 
#include <complex>


namespace HN {
  template <typename, typename>
  class HopfieldNetwork;
}
namespace CSP {

template <typename T = int8_t> 
class CoherenceSetPattern {

 template<typename NT, typename MT> friend class HN::HopfieldNetwork;


  private:
  TP::TrainingPattern<T> tp_;
  NP::NoisyPattern<T> np_;
  EP::EvolvingPattern<T> ep_;
  size_t cols_;
  size_t rows_;
   EP::EvolvingPattern<T>& getEvolvingPattern()  noexcept{return ep_;}
  //  EP::EvolvingPattern<T> getEvolvingPatternCopy()  noexcept{return ep_;}

public:
  explicit CoherenceSetPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows);

  void regrid(const std::size_t numColumns, const std::size_t numRows);
  void reCorrupt(const float noise=0.1f);
  void flipNoisyPixel(std::size_t pos);
  void updateEvolvingState(const std::vector<T>& newPattern);//function that updates ep_
  
  const TP::TrainingPattern<T>& getTrainingPattern() const noexcept;
  const NP::NoisyPattern<T>& getNoisyPattern() const noexcept;
  const EP::EvolvingPattern<T>& getEvolvingPattern() const noexcept;
  

  const std::vector<T>& getTrainingPatternVector() const noexcept;
  const std::vector<T>& getNoisyPatternVector() const noexcept;
  const std::vector<T>& getEvolvingPatternVector() const noexcept;
  const std::vector<float>& getEnergy() const noexcept;
  const size_t& getRow() const noexcept;
  const size_t& getCol() const noexcept;



  bool hasSameDimensionOf(CoherenceSetPattern& anotherCoherenceSetPattern);
};

template class CoherenceSetPattern<int8_t>;
template class CoherenceSetPattern<int>;
template class CoherenceSetPattern<std::complex<int>>;
template class CoherenceSetPattern<std::complex<int8_t>>;

} //namespace

#endif //COHERENCE_SET_PATTERN_HPP