#ifndef QUADRO_HPP
#define QUADRO_HPP
#include <vector>
#include <array>
 #include "HopfieldAlgorithm.h"
#include <cstdint>
#include <string>


namespace Quadro{


class Quadro
{
std::string path_;
int w_;
int h_;
std::vector<int> pattern_;
public:
  Quadro(std::string a, int b, int c);
  std::vector<int> getPattern();
  void updatePattern(int a, int b); 
};





}

#endif