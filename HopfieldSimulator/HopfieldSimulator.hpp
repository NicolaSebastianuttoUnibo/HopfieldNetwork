#ifndef HOPFIELD_SIMULATOR_HPP
#define HOPFIELD_SIMULATOR_HPP

#include "HopfieldNetwork.hpp"
#include "CoherenceSetPattern.hpp"

#include <mutex>
#include <vector>

#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>

namespace HS {


template <typename neurons_type = int8_t, typename matrix_type = double> 
  class HopfieldSimulator {

    std::vector<std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>> patterns_;
  HN::HopfieldNetwork<neurons_type,matrix_type> hn_;
  mutable std::recursive_mutex mtx_;
  
public:
  HopfieldSimulator() = default;
  
  const std::vector<std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>>& getPatterns() const;

  void clear(const int index);  
  void corruptPattern(const size_t index, const float noise=0.1f);
  void emplace_pattern(const std::string& path, const int a, const int b);
  void emplace_pattern(const float noise, const int a, const int b);
  void flipPixelOnPattern(const size_t index, const size_t pixelPos);
  void generatePattern(const float noise, const std::size_t numColumns, const std::size_t numRows);
  void regrid(const size_t numColumns, const size_t numRows);
  void removePattern(const size_t index);
  void resolvePattern(const int index, float* status);
  void saveFileTraining(const std::string& str_buffer);
  void setTraining(const int numColumns, const int numRows, std::vector<matrix_type>& matrix );
  void trainNetworkHebb(float* status=nullptr);
  void trainNetworkWithPseudoinverse(float* status=nullptr);





  size_t size() const;

  bool checkDimension();


};

template class HopfieldSimulator<int8_t,float>;
template class HopfieldSimulator<int8_t,double>;
template class HopfieldSimulator<int,float>;
template class HopfieldSimulator<int,double>;
template class HopfieldSimulator<std::complex<int8_t>,std::complex<double>>;




} //namespace CPS

#endif //HOPFIELD_SIMULATOR_HPP