#ifndef HOPFIELD_SIMULATOR_HPP
#define HOPFIELD_SIMULATOR_HPP
#include "HopfieldNetwork.hpp"
#include "CoherenceSetPattern.hpp"
#include <vector>

namespace HS {

class HopfieldSimulator {

  std::vector<CSP::CoherenceSetPattern> patterns_;
  std::vector<bool> isStateEvolving_;
  HN::HopfieldNetwork hn_;
  
  bool isHopfieldGoing() const;
  
public:
  HopfieldSimulator() = default;
  
  void push_back(CSP::CoherenceSetPattern& pattern);
  void regrid(const size_t numColumns, const size_t numRows);
  void corruptPattern(const size_t index);
  void flipPixelOnPattern(const size_t index, const size_t pixelPos);
  size_t size() const;
  void removePattern(const size_t index);
  const std::vector<CSP::CoherenceSetPattern>& getPatterns() const;
  void trainNetwork();
  


void resolvePattern(const int index);

};
} //namespace CPS

#endif //HOPFIELD_SIMULATOR_HPP