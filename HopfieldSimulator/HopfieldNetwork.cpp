

#include "HopfieldNetwork.hpp"

template <typename neurons_type, typename matrix_type> 
    void HN::HopfieldNetwork<neurons_type,matrix_type>::setTraining(std::vector<matrix_type>& matrix){
weightMatrix_=matrix;
    }


template <typename neurons_type, typename matrix_type> 
     const  std::vector<matrix_type>& HN::HopfieldNetwork<neurons_type,matrix_type>::getTraining() const{
return weightMatrix_;
    }


template <typename neurons_type, typename matrix_type> 
    const std::vector<neurons_type> HN::HopfieldNetwork<neurons_type,matrix_type>::resolvePattern(const std::vector<neurons_type>& array){
        
if(array.size()* array.size()!=weightMatrix_.size()){
        throw std::invalid_argument(("the pattern cannot be resolved due to dimensions incompatibility: m: "+std::to_string(weightMatrix_.size())+" a: "+std::to_string(array.size())).c_str());
}
std::vector<neurons_type> returnVector;

auto iterator=array.begin();
int sum=0;
for (const auto &element : weightMatrix_) {
sum+=static_cast<double>(*iterator)*element;
  iterator++;

if(iterator==array.end()){returnVector.push_back(static_cast<neurons_type>(sum>0?+1:-1));iterator=array.begin();sum=0;}
}
return returnVector;
  
    }
