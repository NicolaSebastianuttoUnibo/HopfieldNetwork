

#include "HopfieldNetwork.hpp"
#include "CoherenceSetPattern.hpp"
#include "EvolvingPattern.hpp"
#include<iostream>

#include<cassert>
#include <Eigen/Dense>


#include <cmath>
#include <algorithm>

template <typename neurons_type, typename matrix_type> 
    void HN::HopfieldNetwork<neurons_type,matrix_type>::trasformEigenInVector(){
    matrix_.resize(W_ij.size());
   std::copy(W_ij.data(), W_ij.data() + W_ij.size(), matrix_.begin());


    }
template <typename neurons_type, typename matrix_type> 
    void HN::HopfieldNetwork<neurons_type,matrix_type>::trasformVectorInEigen(){
  
  const int dim =std::sqrt(matrix_.size());
        W_ij.resize(dim, dim);
    W_ij = Eigen::Map<Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic>>(
        matrix_.data(), dim, dim
    );


    }
    



template <typename neurons_type, typename matrix_type> 
    void HN::HopfieldNetwork<neurons_type,matrix_type>::setTraining(std::vector<matrix_type>& matrix){
matrix_=matrix;
       trasformVectorInEigen();

    }


template <typename neurons_type, typename matrix_type> 
     const  std::vector<matrix_type>& HN::HopfieldNetwork<neurons_type,matrix_type>::getTraining() {
trasformEigenInVector();
     return  matrix_;

   
    }

template <typename neurons_type, typename matrix_type>
const std::vector<neurons_type> HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(const std::vector<neurons_type>& array) {
    
    const int num_neurons = array.size();
    if (num_neurons * num_neurons != W_ij.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(W_ij.size()) + " a: " + std::to_string(num_neurons));
    }

    std::vector<neurons_type> evolving_state = array;

    for (int i = 0; i < num_neurons; ++i) {
        
       
        matrix_type sum = 0;
        for (int j = 0; j < num_neurons; ++j) {
            sum += static_cast<matrix_type>(W_ij(i,j)) * evolving_state[j];
        }

        evolving_state[i] = static_cast<neurons_type>(sum > 0 ? +1 : -1);
    }

    return evolving_state;
}



template <typename neurons_type, typename matrix_type>
   float HN::HopfieldNetwork<neurons_type, matrix_type>::calculateDeltaEnergy(std::vector<neurons_type>& input, const int index){
const int num_neurons =input.size();

        if (num_neurons * num_neurons != W_ij.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(W_ij.size()) + " a: " + std::to_string(num_neurons));
    }
float delta=0;
  for (int i = 0; i < num_neurons; ++i) {
  delta-=(static_cast<float>(input[i]*input[index])*static_cast<float>(W_ij(i,index)));
}
return delta;
   }

template <typename neurons_type, typename matrix_type>
   float HN::HopfieldNetwork<neurons_type, matrix_type>::calculateEnergy(std::vector<neurons_type>& input){


    
const int num_neurons =input.size();

        if (num_neurons * num_neurons != W_ij.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(matrix_.size()) + " a: " + std::to_string(num_neurons));
    }
float energy=0;
  for (int i = 1; i < num_neurons; ++i) {
  for (int j = i+1; j < num_neurons; ++j) {
energy-=(static_cast<float>(input[i]*input[j])*static_cast<float>(W_ij(i,j)));
  }}

    return energy;
   }



template <typename neurons_type, typename matrix_type>
   void  HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(CSP::CoherenceSetPattern<neurons_type>& cps, float* status){

    EP::EvolvingPattern<neurons_type>& ep=cps.getEvolvingPattern();
  std::vector<neurons_type>& getVector=ep.getPattern();
  std::vector<float>& getEnergy=ep.getEnergy();
const int num_neurons =getVector.size();


 const int totalIteration=(num_neurons)*num_neurons;
int count=0;

    if (num_neurons * num_neurons != W_ij.size()) {

        throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(W_ij.size()) + " a: " + std::to_string(num_neurons));
    }
    getEnergy.clear();
    getEnergy.reserve(num_neurons+10);
    getEnergy.push_back(calculateEnergy(getVector));

float thisdelta=0;
  for (int i = 0; i < num_neurons; ++i) {
            
       thisdelta=0;
        matrix_type sum = 0;
        for (int j = 0; j < num_neurons; ++j) {
if(*status<0){return;}
 count++;

thisdelta-=(static_cast<float>(getVector[i]*getVector[j])*static_cast<float>(W_ij(i,j)));
           
               *status = static_cast<float>(count) / totalIteration;
             sum += static_cast<matrix_type>(W_ij(i,j)) * getVector[j];
       
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