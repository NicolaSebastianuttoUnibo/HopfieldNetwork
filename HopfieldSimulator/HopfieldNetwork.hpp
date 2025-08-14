#ifndef HOPFIELD_NETWORK_HPP
#define HOPFIELD_NETWORK_HPP
#include <vector> 
#include <numeric> 
#include <string> 
#include "CoherenceSetPattern.hpp"


namespace CSP {
    template <typename T>
    class CoherenceSetPattern;
}
namespace HN{


template <typename neurons_type = int8_t, typename matrix_type = double> 

class HopfieldNetwork{
    private:
    std::vector<matrix_type> weightMatrix_ ;

    public:
    HopfieldNetwork() = default;
    void setTraining(std::vector<matrix_type>& matrix);
    const std::vector<matrix_type>& getTraining()const;


    const std::vector<neurons_type> resolvePattern(const std::vector<neurons_type>& array);
   void  resolvePattern(CSP::CoherenceSetPattern<neurons_type>& cps, float* status=nullptr);
  
   float  calculateEnergy(std::vector<neurons_type>& input);
   float  calculateDeltaEnergy(std::vector<neurons_type>& input, const size_t index);

   template<typename Iterable, typename Extractor>
void trainNetwork(const Iterable &patterns, const Extractor extractor, float* status=nullptr) {
   
    if (patterns.empty()) {return;}
    const int numberNeurons = extractor(*patterns.begin()).size();
 const matrix_type norm_factor = static_cast<matrix_type>(1.0) / static_cast<matrix_type>(numberNeurons);


 const int totalIteration=(numberNeurons+1)*numberNeurons*patterns.size()/2;
int count=0;
    weightMatrix_.assign(numberNeurons * numberNeurons, 0.0);

    for (const auto& pattern_container : patterns) {
        const auto& p = extractor(pattern_container);
        

        for (int i = 0; i < numberNeurons; ++i) {
            for (int j = i; j < numberNeurons; ++j) {
                count++;
               *status = static_cast<float>(count) / totalIteration;
                if(i==j){continue;}
                  matrix_type product_p_ij = static_cast<matrix_type>(p[i] * p[j]);
            weightMatrix_[i * numberNeurons + j] += product_p_ij * norm_factor;
            weightMatrix_[j * numberNeurons + i] += product_p_ij * norm_factor;
            
            }
        }
    }
}
    
   

   
};//class


template class HopfieldNetwork<int8_t,float>;
template class HopfieldNetwork<int8_t,double>;
template class HopfieldNetwork<int,float>;
template class HopfieldNetwork<int,double>;

} //namespace HN


#endif  //HOPFIELD_NETWORK_HPP