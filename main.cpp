#include <iostream>
#include <vector>
 #include "HopfieldAlgorithm.h"
 #include "corrotto.h"

int main(){
    std::vector<int> a={2,4};
Corrotto::Corrotto c(a);
std::cout<<c.size();
}