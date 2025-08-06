#ifndef CORROTTO_HPP
#define CORROTTO_HPP
#include<vector>

namespace Corrotto{


class Corrotto
{

std::vector<int> pattern_;
  std::vector<int> Corrompi(std::vector<int>);

public:

  Corrotto(std::vector<int> a);
 int size(){
    return pattern_.size();
 }


};





}

#endif