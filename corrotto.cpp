#include "corrotto.h"
#include<vector>
#include<iostream>




  Corrotto::Corrotto::Corrotto(std::vector<int> a){
    Corrotto::Corrotto::pattern_.clear(); ///forse è inutile
    Corrotto::Corrotto::pattern_= Corrotto::Corrotto::Corrompi(a);

}





  std::vector<int>  Corrotto::Corrotto::Corrompi(std::vector<int> a){
a[0]=1-a[0];
return a;
  }

  int Corrotto::Corrotto::size(){
    return Corrotto::Corrotto::pattern_.size();
  }
  std::vector<int>& Corrotto::Corrotto::getPattern(){

    return Corrotto::Corrotto::pattern_;
  }

  void Corrotto::Corrotto::CorrompiPixel(int index){
Corrotto::Corrotto::pattern_[index]=1-Corrotto::Corrotto::pattern_[index];

    }
