#include "corrotto.h"
#include<vector>




  Corrotto::Corrotto::Corrotto(std::vector<int> a){
    Corrotto::Corrotto::pattern_.clear(); 
    Corrotto::Corrotto::pattern_= Corrotto::Corrotto::Corrompi(a);

}


  std::vector<int>  Corrotto::Corrotto::Corrompi(std::vector<int> a){
a[0]=-a[0];
return a;
  }