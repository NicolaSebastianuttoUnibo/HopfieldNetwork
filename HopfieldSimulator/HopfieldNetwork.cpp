

#include "HopfieldNetwork.hpp"
#include<iostream>
template <typename neurons_type, typename matrix_type> 
    void HN::HopfieldNetwork<neurons_type,matrix_type>::setTraining(std::vector<matrix_type>& matrix){
weightMatrix_=matrix;
    }


template <typename neurons_type, typename matrix_type> 
     const  std::vector<matrix_type>& HN::HopfieldNetwork<neurons_type,matrix_type>::getTraining() const{
return weightMatrix_;
    }

template <typename neurons_type, typename matrix_type>
const std::vector<neurons_type> HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(const std::vector<neurons_type>& array) {
    
    const size_t num_neurons = array.size();
    if (num_neurons * num_neurons != weightMatrix_.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(weightMatrix_.size()) + " a: " + std::to_string(num_neurons));
    }

    std::vector<neurons_type> evolving_state = array;

    for (size_t i = 0; i < num_neurons; ++i) {
        
       
        matrix_type sum = 0;
        for (size_t j = 0; j < num_neurons; ++j) {
            sum += static_cast<matrix_type>(weightMatrix_[i * num_neurons + j]) * evolving_state[j];
        }

        evolving_state[i] = static_cast<neurons_type>(sum > 0 ? +1 : -1);
    }

    return evolving_state;
}