#ifndef PATTERN_HPP
#define PATTERN_HPP
#include "corrotto.h"
#include "quadro.h"
#include <vector>
namespace Pattern {

class Pattern {
  std::vector<int> arraydinamico_;
  Quadro::Quadro q_;
  Corrotto::Corrotto c_;

public:
  Pattern(std::string &a, int b, int c);
  void applicaPixelatura(int a, int b);
  void Corrompi();
  void CorrompiPixel(int pos);
  std::vector<int> &getPatternQ();
  std::vector<int> &getPatternC();
  std::vector<int> &getPatternD();
};

} 

#endif