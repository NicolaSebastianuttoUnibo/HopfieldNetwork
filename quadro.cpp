#include "quadro.h"

Quadro::Quadro::Quadro(std::string str, int a, int b): path_{str},w_{a},h_{b} {

Quadro::Quadro::updatePattern(a,b);
}

  std::vector<int> Quadro::Quadro::getPattern(){
    return pattern_;
  }

  void Quadro::Quadro::updatePattern(int a,int b){


    Quadro::Quadro::pattern_.clear();
for(int i=0;i<a*b;i++){
    Quadro::Quadro::pattern_.push_back(i);
}
  }

