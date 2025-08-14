

#include "HopfieldNetwork.hpp"
#include "CoherenceSetPattern.hpp"
#include "EvolvingPattern.hpp"
#include<iostream>

#include<cassert>


#include <cmath>
#include <algorithm> // Per std::max
bool areAlmostEqual(float a, float b, float relative_tolerance = 0.05f, float absolute_tolerance = 1e-9f) {
    // Se la differenza è inferiore alla tolleranza assoluta, sono considerati uguali.
    // Questo gestisce il caso in cui a e b sono molto vicini a zero.
    float diff = std::abs(a - b);
    if (diff <= absolute_tolerance) {
        return true;
    }

    // Altrimenti, usa la tolleranza relativa.
    // Dividiamo per il più grande dei due valori per rendere il test simmetrico e più stabile.
    return diff <= (std::max(std::abs(a), std::abs(b)) * relative_tolerance);
}



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



template <typename neurons_type, typename matrix_type>
   float HN::HopfieldNetwork<neurons_type, matrix_type>::calculateDeltaEnergy(std::vector<neurons_type>& input, const size_t index){
const size_t num_neurons =input.size();

        if (num_neurons * num_neurons != weightMatrix_.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(weightMatrix_.size()) + " a: " + std::to_string(num_neurons));
    }
float delta=0;
  for (size_t i = 0; i < num_neurons; ++i) {
  delta-=(static_cast<float>(input[i]*input[index])*static_cast<float>(weightMatrix_[index*num_neurons+i]));
}
return delta;
   }

template <typename neurons_type, typename matrix_type>
   float HN::HopfieldNetwork<neurons_type, matrix_type>::calculateEnergy(std::vector<neurons_type>& input){


    
const size_t num_neurons =input.size();

        if (num_neurons * num_neurons != weightMatrix_.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(weightMatrix_.size()) + " a: " + std::to_string(num_neurons));
    }
float energy=0;
  for (size_t i = 1; i < num_neurons; ++i) {
  for (size_t j = i+1; j < num_neurons; ++j) {
energy-=(static_cast<float>(input[i]*input[j])*static_cast<float>(weightMatrix_[i*num_neurons+j]));
  }}

    return energy;
   }



template <typename neurons_type, typename matrix_type>
   void  HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(CSP::CoherenceSetPattern<neurons_type>& cps, float* status){
//  std::vector<neurons_type> myvector=cps.getEvolvingPatternCopy().getPattern();
 EP::EvolvingPattern<neurons_type>& ep=cps.getEvolvingPattern();
  std::vector<neurons_type>& getVector=ep.getPattern();
  std::vector<float>& getEnergy=ep.getEnergy();
const size_t num_neurons =getVector.size();


 const int totalIteration=(num_neurons)*num_neurons;
int count=0;

    if (num_neurons * num_neurons != weightMatrix_.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(weightMatrix_.size()) + " a: " + std::to_string(num_neurons));
    }
    getEnergy.clear();
    getEnergy.reserve(num_neurons);
    getEnergy.push_back(calculateEnergy(getVector));

float thisdelta=0;
  for (size_t i = 0; i < num_neurons; ++i) {
            
       thisdelta=0;
        matrix_type sum = 0;
        for (size_t j = 0; j < num_neurons; ++j) {
if(*status<0){return;}
 count++;

thisdelta-=(static_cast<float>(getVector[i]*getVector[j])*static_cast<float>(weightMatrix_[i*num_neurons+j]));
           
               *status = static_cast<float>(count) / totalIteration;
             sum += static_cast<matrix_type>(weightMatrix_[i * num_neurons + j]) * getVector[j];
       
            }///fine  indice j

         getVector[i] = static_cast<neurons_type>(sum > 0 ? +1 : -1);
         getEnergy.push_back(getEnergy.back()+calculateDeltaEnergy(getVector,i)-thisdelta );

          

    }/// fine indice i 


  

   }


   template <typename neurons_type, typename matrix_type> 
 void  HN::HopfieldNetwork<neurons_type,matrix_type>::clearEnergy(CSP::CoherenceSetPattern<neurons_type>& cps){
 EP::EvolvingPattern<neurons_type>& ep=cps.getEvolvingPattern();
ep.clearEnergy();
   }