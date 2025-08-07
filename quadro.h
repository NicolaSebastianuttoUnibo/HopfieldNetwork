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
std::vector<int> pattern_;
public:
   Quadro(std::string a, int b, int c);
   Quadro()=default;
  std::vector<int>& getPattern();
  void updatePattern(int a, int b); 

};





}

#endif