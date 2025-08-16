

#include "HopfieldNetwork.hpp"
#include "CoherenceSetPattern.hpp"
#include "EvolvingPattern.hpp"
#include<iostream>

#include<cassert>
#include <Eigen/Dense>


#include <cmath>
#include <algorithm>
#include <complex>
#include <Eigen/Dense>
#include "cast.hpp"

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

// template <typename neurons_type, typename matrix_type>
// const std::vector<neurons_type> HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(const std::vector<neurons_type>& array) {
    
//     const int num_neurons = array.size();
//     if (num_neurons * num_neurons != W_ij.size()) {

//         throw std::invalid_argument("the pattern cannot be resolved due to dimensions incompatibility: m: " + std::to_string(W_ij.size()) + " a: " + std::to_string(num_neurons));
//     }

//     std::vector<neurons_type> evolving_state = array;

//     for (int i = 0; i < num_neurons; ++i) {
        
       
//         matrix_type sum = 0;
//         for (int j = 0; j < num_neurons; ++j) {
//             sum += static_cast<matrix_type>(W_ij(i,j)) * evolving_state[j];
//         }

//         evolving_state[i] = static_cast<neurons_type>(sum > 0 ? +1 : -1);
//     }

//     return evolving_state;
// }


template <typename neurons_type, typename matrix_type>
float HN::HopfieldNetwork<neurons_type, matrix_type>::calculateEnergy(const std::vector<neurons_type>& input) {
    const int num_neurons = input.size();

    // if (num_neurons != W_ij.rows() || num_neurons != W_ij.cols()) {
    //     throw std::invalid_argument("Dimensioni incompatibili tra pattern e matrice dei pesi.");
    // }

    // Eigen::Matrix<matrix_type, Eigen::Dynamic, 1> s(num_neurons);
    // for (int i = 0; i < num_neurons; ++i) {
    //     s(i) = static_cast<matrix_type>(input[i]);
    // }

    // auto energy_complex = s.adjoint() * W_ij * s;
    // return -0.5f * energy_complex(0, 0).real();
    return 0.0f;
}


template <typename neurons_type, typename matrix_type>
void HN::HopfieldNetwork<neurons_type, matrix_type>::resolvePattern(
    CSP::CoherenceSetPattern<neurons_type>& cps, float* status) {


    float count=0;
        EP::EvolvingPattern<neurons_type>& ep=cps.getEvolvingPattern();
  std::vector<neurons_type>& getVector=ep.getPattern();
  std::vector<float>& getEnergy=ep.getEnergy();
const int num_neurons =getVector.size();


    if (num_neurons != W_ij.rows()) {
        throw std::invalid_argument("Dimensioni pattern/matrice non compatibili.");
    }

    getEnergy.clear();
    getEnergy.reserve(num_neurons);
count++;
    getEnergy.push_back(calculateEnergy(getVector));
     for (int i = 0; i < num_neurons; ++i) {
        neurons_type best_point;
        matrix_type sum=0; 
     for (int j = 0; j < num_neurons; ++j) {
     sum+=(W_ij(i,j)*custom_cast<matrix_type>(getVector[j]));
     std::cout<<W_ij(i,j)<<"\n";

}////for j
     double min_dist_sq = std::numeric_limits<double>::max();
     
    for (const auto& candidate_point : POINTS) {
     double dist_sq = std::norm(sum - custom_cast<matrix_type>(candidate_point));

        // std::cout<<static_cast<int>(sum)<<","<<static_cast<int>(candidate_point)<<"\n";

    if (dist_sq < min_dist_sq) {
        min_dist_sq = dist_sq;
        best_point = candidate_point;
    }

 }
 getVector[i]=best_point;
*status++;   
    }///for i

   
    // for (int i = 0; i < num_neurons; ++i) {
    //     if (status && *status < 0) return;


    //     matrix_type local_field;
    //     for (int j = 0; j < num_neurons; ++j) {
    //         local_field += W_ij(i, j) * static_cast<matrix_type>(pattern_vector[j]);
    //     }
        
    //     const neurons_type old_state = pattern_vector[i];

    
    //     neurons_type new_state = POINTS[0];
    //     double min_dist_sq = std::norm(local_field - static_cast<matrix_type>(new_state));

    //     for (size_t k = 1; k < POINTS.size(); ++k) {
    //         const auto& candidate_point = POINTS[k];
    //         double dist_sq = std::norm(local_field - static_cast<matrix_type>(candidate_point));
            
    //         if (dist_sq < min_dist_sq) {
    //             min_dist_sq = dist_sq;
    //             new_state = candidate_point;
    //         }
    //     }
        
       
    //     pattern_vector[i] = new_state;

      
    //     matrix_type state_change = static_cast<matrix_type>(new_state) - static_cast<matrix_type>(old_state);
    //     float delta_E = -std::real(std::conj(state_change) * local_field);

    //     energy_history.push_back(energy_history.back() + delta_E);

    //     if (status) {
    //         *status = static_cast<float>(i + 1) / num_neurons;
    //     }
    // }
}


   template <typename neurons_type, typename matrix_type> 
 void  HN::HopfieldNetwork<neurons_type,matrix_type>::clearEnergy(CSP::CoherenceSetPattern<neurons_type>& cps){
 EP::EvolvingPattern<neurons_type>& ep=cps.getEvolvingPattern();
ep.clearEnergy();
   }