#ifndef HOPFIELD_NETWORK_HPP
#define HOPFIELD_NETWORK_HPP

#include <Eigen/Dense>
#include "math/Cast.hpp"
#include "CoherenceSetPattern.hpp"

#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>

#include <vector> 

namespace CSP {
    template <typename T>
    class CoherenceSetPattern;
}
namespace HN{


template <typename neurons_type = int8_t, typename matrix_type = double> 

class HopfieldNetwork{
    private:
    std::array<T,MD::getMathematicalNumberVertex<T>()>POINTS =
    MD::getMathematicalVertex<T>();
    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> W_ij;
    std::vector<matrix_type> matrix_;
    void trasformEigenInVector();
    void trasformVectorInEigen();
const matrix_type localField(const int index, const std::vector<neurons_type>& input);

    public:

    HopfieldNetwork() = default;
     const std::vector<matrix_type>& getTraining();
     void setTraining(std::vector<matrix_type>& matrix);


   float calculateEnergy(const std::vector<neurons_type>& input);
   void  clearEnergy(CSP::CoherenceSetPattern<neurons_type>& cps);
    const std::vector<neurons_type> resolvePattern(const std::vector<neurons_type>& array) const;
    void  resolvePattern(CSP::CoherenceSetPattern<neurons_type>& cps, float* status=nullptr);





   template<typename Iterable, typename Extractor>
void trainNetworkWithPseudoinverse(const Iterable& patterns, const Extractor extractor, float* status = nullptr);
   template<typename Iterable, typename Extractor>
void trainNetworkWithHebb(const Iterable& patterns, const Extractor extractor, float* status = nullptr);
   





};// end class

template <typename neurons_type, typename matrix_type>
template<typename Iterable, typename Extractor>

void HN::HopfieldNetwork<neurons_type, matrix_type>::trainNetworkWithHebb(const Iterable& patterns, const Extractor extractor, float* status) {
   
    if (patterns.empty()) {return;}
    const int numberNeurons = extractor(*patterns.begin()).size();
    const matrix_type norm_factor = static_cast<matrix_type>(1.0) / static_cast<matrix_type>(numberNeurons);

   const int totalIteration=patterns.size();
   int count=0;
    
    W_ij = Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic>::Zero(numberNeurons, numberNeurons);

    for (const auto& pattern_container : patterns) {
        const auto& p_std = extractor(pattern_container);
        Eigen::Matrix<matrix_type, Eigen::Dynamic, 1> p(numberNeurons); 
        for (int k = 0; k < numberNeurons; ++k) {
            p[k] = custom_cast<matrix_type>(p_std[k]); 
        }

   W_ij += (p * p.adjoint()) * norm_factor;
   if(status){
                if(*status<0){
                  return;
                }
                count++;
            *status = static_cast<float>(count) / totalIteration;}

    }///for patterns
       W_ij.diagonal().setZero();

        // for (int i = 0; i < numberNeurons; ++i) {
        //     for (int j = i; j < numberNeurons; ++j) {
                
        //         if(status!=nullptr){
        //         if(*status<0){
        //           return;
        //         }
        //            count++;
        //            *status = static_cast<float>(count) / totalIteration;
        //         }
                
               
        //         if(i==j){continue;}
        //           matrix_type product_p_ij = static_cast<matrix_type>(p[i] * p[j]);
        //     matrix_[i * numberNeurons + j] += product_p_ij * norm_factor;
        //     matrix_[j * numberNeurons + i] += product_p_ij * norm_factor;
            
        //     }
        // }
        // trasformVectorInEigen();



    
}



template <typename neurons_type, typename matrix_type>
template <typename Iterable, typename Extractor>
void HN::HopfieldNetwork<neurons_type, matrix_type>::trainNetworkWithPseudoinverse(const Iterable& patterns, const Extractor extractor, float* status) {

    const size_t numPatterns = patterns.size();
    const size_t numNeurons = extractor(*patterns.begin()).size();

    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> X(numNeurons, numPatterns);
    int p_idx = 0;
    for (const auto& pattern_container : patterns) {
        const auto& p = extractor(pattern_container);
        for (size_t i = 0; i < numNeurons; ++i) {
            X(i, p_idx) = custom_cast<matrix_type>(p[i]); 
        }
        p_idx++;
    }

    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> C = X.adjoint() * X;
    if (C.determinant() == matrix_type(0.0)) { 
        throw std::runtime_error("I pattern sono linearmente dipendenti, la matrice di correlazione non è invertibile.");
    }
    
    Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic> C_inv = C.inverse();
    
    if (status) *status = 0.1f;

    auto T = X * C_inv;

    if (status) *status = 0.5f;

    auto X_H = X.adjoint(); 
    if (status) *status = 0.75f;

    W_ij.resize(numNeurons, numNeurons); 

    for (size_t i = 0; i < numNeurons; ++i) {
        if (status && *status < 0) return;  

        W_ij.row(i) = T.row(i) * X_H; 
        
if (status) {
            if(*status<0){return;}
            *status = (static_cast<float>(i) / numNeurons) ;
        }


    }
    
    
}



template class HopfieldNetwork<int8_t,float>;
template class HopfieldNetwork<int8_t,double>;
template class HopfieldNetwork<std::complex<int8_t>,std::complex<double>>;
template class HopfieldNetwork<int,float>;
template class HopfieldNetwork<int,double>;

} //namespace HN


#endif  //HOPFIELD_NETWORK_HPP