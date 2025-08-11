#include "HopfieldNetwork.hpp"


    void HN::HopfieldNetwork::setTraining(std::vector<double>& matrix){
weightMatrix_=matrix;
    }

     const  std::vector<double>& HN::HopfieldNetwork::getTraining() const{
return weightMatrix_;
    }
