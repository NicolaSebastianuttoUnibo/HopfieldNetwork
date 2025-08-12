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


    //Train Network
    template<typename Iterable, typename Extractor>
    void trainNetwork(const Iterable &patterns, const Extractor extractor){
 if (patterns.empty() || extractor(*patterns.begin()).empty()) {
        return;
    }

    const int numberNeurons = extractor(*patterns.begin()).size();

    weightMatrix_.clear();
weightMatrix_.assign(numberNeurons * numberNeurons, 0.0);

for (int i = 0; i < numberNeurons; ++i) {
            for (int j = i; j < numberNeurons; ++j) {
                if (i == j) {
                    continue;
                }
                else{
                double weight_ij=std::accumulate(
                  patterns.begin(),patterns.end(),0.0,
                  [&](double sum, const auto& pattern){
                    auto thisvector=extractor(pattern);
                    return sum+(thisvector[i]*thisvector[j]);}
                  );
                   weightMatrix_[i+j*numberNeurons]=weight_ij;
                   weightMatrix_[j+i*numberNeurons]=weight_ij;

                }
            }}
    }
    
   const std::vector<neurons_type> resolvePattern(const std::vector<neurons_type>& array);
  

   
};//class


template class HopfieldNetwork<int8_t,float>;
template class HopfieldNetwork<int8_t,double>;
template class HopfieldNetwork<int,float>;
template class HopfieldNetwork<int,double>;

} //namespace HN


#endif  //HOPFIELD_NETWORK_HPP