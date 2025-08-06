 #include "HopfieldAlgorithm.h"
 #include <array>
 #include <iostream>

 
void Hopfield::HopfieldAlgorithm::saveArrays(std::vector<std::vector<int>> a){

   Hopfield::HopfieldAlgorithm::matrix.clear(); 

for(int i=0;i<a.size();i++){
    

for(int j=0;j<a[i].size();j++){
Hopfield::HopfieldAlgorithm::matrix.push_back(a[i][j]);
}}
    }

        std::vector<int> Hopfield::HopfieldAlgorithm::disicorrompi(std::vector<int> array){
Hopfield::HopfieldAlgorithm::matrix;
std::vector<int> result;
for(int i=0;i<array.size();i++){
    int sum=0;
    for(int j=0;j<array.size();j++){
        sum+=(Hopfield::HopfieldAlgorithm::matrix[j+i*array.size()]*array[j]);
    }
    result.push_back(sum);

}
return result;
        }
