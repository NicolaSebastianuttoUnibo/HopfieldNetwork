#ifndef HOPFIELD_NETWORK_HPP
#define HOPFIELD_NETWORK_HPP
#include <stdexcept> 
#include <vector> 
#include <numeric> 
#include <string> 
namespace HN{


template <typename neurons_type = int8_t, typename matrix_type = double> 

class HopfieldNetwork{
    private:
    std::vector<matrix_type> weightMatrix_ ;

    public:
    HopfieldNetwork() = default;
    void setTraining(std::vector<matrix_type>& matrix);
    const std::vector<matrix_type>& getTraining()const;


   template<typename Iterable, typename Extractor>
void trainNetwork(const Iterable &patterns, const Extractor extractor) {
    if (patterns.empty()) {return;}
    const int numberNeurons = extractor(*patterns.begin()).size();

    weightMatrix_.assign(numberNeurons * numberNeurons, 0.0);

    for (const auto& pattern_container : patterns) {
        const auto& p = extractor(pattern_container);
        
        // Aggiungi il contributo di questo pattern a tutta la matrice dei pesi
        for (int i = 0; i < numberNeurons; ++i) {
            for (int j = i; j < numberNeurons; ++j) {
                if(i==j){continue;}
                weightMatrix_[i * numberNeurons + j] += static_cast<matrix_type>(p[i]) * p[j]/numberNeurons;
                weightMatrix_[j * numberNeurons + i] += static_cast<matrix_type>(p[i]) * p[j]/numberNeurons;
            
            }
        }
    }
}
    
   const std::vector<neurons_type> resolvePattern(const std::vector<neurons_type>& array);
  

   
};//class


template class HopfieldNetwork<int8_t,float>;
template class HopfieldNetwork<int8_t,double>;
template class HopfieldNetwork<int,float>;
template class HopfieldNetwork<int,double>;

} //namespace HN


#endif  //HOPFIELD_NETWORK_HPP