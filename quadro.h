#ifndef QUADRO_HPP
#define QUADRO_HPP
#include "HopfieldAlgorithm.h"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace Quadro {

class Quadro {
  std::string path_;
  std::vector<int> pattern_;

public:
  Quadro(std::string &a, int b, int c);
  Quadro() = default;
  std::vector<int> &getPattern();
  void updatePattern(int a, int b);
};

} 

#endif