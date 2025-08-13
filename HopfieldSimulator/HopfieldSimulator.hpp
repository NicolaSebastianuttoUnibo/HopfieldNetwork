#ifndef HOPFIELD_SIMULATOR_HPP
#define HOPFIELD_SIMULATOR_HPP
#include "HopfieldNetwork.hpp"
#include "CoherenceSetPattern.hpp"
#include <vector>


namespace HS {


template <typename neurons_type = int8_t, typename matrix_type = double> 
  class HopfieldSimulator {

  std::vector<CSP::CoherenceSetPattern<neurons_type>> patterns_;
  std::vector<bool> isStateEvolving_;
  HN::HopfieldNetwork<neurons_type,matrix_type> hn_;
  
  size_t cols_;
  size_t rows_;

  bool isHopfieldGoing() const;
  bool check_=true;
  
public:
  HopfieldSimulator() = default;
  
  void push_back(CSP::CoherenceSetPattern<neurons_type>& pattern);
  void regrid(const size_t numColumns, const size_t numRows);
  void corruptPattern(const size_t index, const float noise=0.1f);
  void flipPixelOnPattern(const size_t index, const size_t pixelPos);
  size_t size() const;
  void removePattern(const size_t index);
  const std::vector<CSP::CoherenceSetPattern<neurons_type>>& getPatterns() const;
  void trainNetwork(float* status=nullptr);
  size_t size();


void resolvePattern(const int index, float* status);

};

template class HopfieldSimulator<int8_t,float>;
template class HopfieldSimulator<int8_t,double>;
template class HopfieldSimulator<int,float>;
template class HopfieldSimulator<int,double>;



} //namespace CPS

#endif //HOPFIELD_SIMULATOR_HPP