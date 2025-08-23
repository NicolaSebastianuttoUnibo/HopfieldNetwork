#ifndef COHERENCE_SET_PATTERN_HPP
#define COHERENCE_SET_PATTERN_HPP

#include "ITrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include "HopfieldNetwork.hpp"

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>

namespace HN {
  template <typename, typename>
  class HopfieldNetwork;
}
namespace CSP {

template <typename T = int8_t> 
class CoherenceSetPattern {


  private:
  std::unique_ptr<TP::ITrainingPattern<T>> tp_;
  NP::NoisyPattern<T> np_;
  EP::EvolvingPattern<T> ep_;
  size_t cols_;
  size_t rows_;
   EP::EvolvingPattern<T>& getEvolvingPattern()  noexcept;

   
public:
  explicit CoherenceSetPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows);
  explicit CoherenceSetPattern(const float noise, const std::size_t numColumns, const std::size_t numRows);
   


  const std::vector<T>& getTrainingPatternVector() const noexcept;
  const std::vector<T>& getNoisyPatternVector() const noexcept;
  const std::vector<T>& getEvolvingPatternVector() const noexcept;
  const std::vector<float>& getEnergy() const noexcept;
   size_t getRow() const noexcept;
   size_t getCol() const noexcept;

  bool hasSameDimensionOf(CoherenceSetPattern& anotherCoherenceSetPattern)const;


  void flipNoisyPixel(std::size_t pos);
  void reCorrupt(const float noise=0.1f);
  void regrid(const std::size_t numColumns, const std::size_t numRows);
  void updateEvolvingState(const std::vector<T>& newPattern);
  

template <typename MatrixType = double> 
void resolveEvolvingPattern(HN::HopfieldNetwork<T, MatrixType>& network, std::atomic<float>* status) {
     
  network.resolvePattern(this->ep_, status);
        }
void clearEnergy() {
            ep_.getEnergy().clear();
        }


};

template class CoherenceSetPattern<int8_t>;
template class CoherenceSetPattern<int>;
template class CoherenceSetPattern<std::complex<int>>;
template class CoherenceSetPattern<std::complex<int8_t>>;

} //namespace

#endif //COHERENCE_SET_PATTERN_HPP