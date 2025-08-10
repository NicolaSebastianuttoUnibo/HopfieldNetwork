#ifndef CORROTTO_HPP
#define CORROTTO_HPP
#include <vector>

namespace Corrotto {

class Corrotto {

  std::vector<int> pattern_;
  std::vector<int> &Corrompi(std::vector<int> &a);

public:
  void CorrompiPixel(int &index);
  Corrotto(std::vector<int> &a);
  Corrotto() = default;
  std::vector<int> &getPattern();
  int size();
};

} 

#endif