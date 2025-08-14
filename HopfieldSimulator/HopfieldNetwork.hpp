#ifndef HOPFIELD_NETWORK_HPP
#define HOPFIELD_NETWORK_HPP
#include <vector> 
#include <numeric> 
#include <string> 
#include <iostream> 
#include "CoherenceSetPattern.hpp"
#include <Eigen/Dense>

namespace CSP {
    template <typename T>
    class CoherenceSetPattern;
}
namespace HN{


template <typename neurons_type = int8_t, typename matrix_type = double> 

class HopfieldNetwork{
    private:
    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> W_ij;
    std::vector<matrix_type> matrix_;
    void trasformEigenInVector();
    void trasformVectorInEigen();
    public:
    HopfieldNetwork() = default;
    void setTraining(std::vector<matrix_type>& matrix);
     const std::vector<matrix_type>& getTraining();


    const std::vector<neurons_type> resolvePattern(const std::vector<neurons_type>& array);
   
   void  clearEnergy(CSP::CoherenceSetPattern<neurons_type>& cps);

   
    void  resolvePattern(CSP::CoherenceSetPattern<neurons_type>& cps, float* status=nullptr);
  
   float  calculateEnergy(std::vector<neurons_type>& input);
   float  calculateDeltaEnergy(std::vector<neurons_type>& input, const int  index);

   template<typename Iterable, typename Extractor>
void trainNetwork(const Iterable &patterns, const Extractor extractor, float* status=nullptr) {
   
    if (patterns.empty()) {return;}
    const int numberNeurons = extractor(*patterns.begin()).size();
 const matrix_type norm_factor = static_cast<matrix_type>(1.0) / static_cast<matrix_type>(numberNeurons);


 const int totalIteration=(numberNeurons+1)*numberNeurons*patterns.size()/2;
int count=0;
    matrix_.assign(numberNeurons * numberNeurons, 0.0);

    for (const auto& pattern_container : patterns) {
        const auto& p = extractor(pattern_container);
        

        for (int i = 0; i < numberNeurons; ++i) {
            for (int j = i; j < numberNeurons; ++j) {
                
                if(status!=nullptr){
                if(*status<0){
 return;
                }
                   count++;
                   *status = static_cast<float>(count) / totalIteration;
                }
                
               
                if(i==j){continue;}
                  matrix_type product_p_ij = static_cast<matrix_type>(p[i] * p[j]);
            matrix_[i * numberNeurons + j] += product_p_ij * norm_factor;
            matrix_[j * numberNeurons + i] += product_p_ij * norm_factor;
            
            }
        }
    }
}
    
   

   


template<typename Iterable, typename Extractor>
void trainNetworkWithPseudoinverse(const Iterable& patterns, const Extractor extractor, float* status = nullptr) {
    if (patterns.empty()) {
        return;
    }

    const size_t numPatterns = patterns.size();
    const size_t numNeurons = extractor(*patterns.begin()).size();


    
    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> X(numNeurons, numPatterns);
    int p_idx = 0;
    for (const auto& pattern_container : patterns) {
        const auto& p = extractor(pattern_container);
        for (size_t i = 0; i < numNeurons; ++i) {
            X(i, p_idx) = static_cast<double>(p[i]);
        }
        p_idx++;
    }


    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> C = X.transpose() * X;


    if (C.determinant() == 0) { 
        throw std::runtime_error("I pattern sono linearmente dipendenti, la matrice di correlazione non è invertibile.");
    }
    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> C_inv = C.inverse();
    
    if (status) *status = 0.1f;

    auto T = X * C_inv;

    if (status) *status = 0.5f;
    auto X_T= X.transpose();
    if (status) *status = 0.75f;

W_ij.resize(numNeurons, numNeurons); 

 for (size_t i = 0; i < numNeurons; ++i) {
        if (status && *status < 0) return;  

        
        

        W_ij.row(i) = T.row(i) * X_T;
        

        if (status) {
            if(*status<0){return;}
            *status = (static_cast<float>(i) / numNeurons) ;
        }
    }



    if (status) *status = 1.0f;
}




};//class


template class HopfieldNetwork<int8_t,float>;
template class HopfieldNetwork<int8_t,double>;
template class HopfieldNetwork<int,float>;
template class HopfieldNetwork<int,double>;

} //namespace HN


#endif  //HOPFIELD_NETWORK_HPP