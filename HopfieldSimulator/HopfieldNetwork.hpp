#ifndef HOPFIELD_NETWORK_HPP
#define HOPFIELD_NETWORK_HPP
#include <stdexcept> 
#include <vector> 
#include <numeric> 
#include <string> 
namespace HN{



class HopfieldNetwork{
    private:
    std::vector<double> weightMatrix_ ;

    public:
    HopfieldNetwork() = default;
    void setTraining(std::vector<double>& matrix);
    const std::vector<double>& getTraining()const;


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
    
    
    
    //resolvePattern
    template<typename type>
    const std::vector<type> resolvePattern(const std::vector<type>& array){
        
if(array.size()* array.size()!=weightMatrix_.size()){
        throw std::invalid_argument(("the pattern cannot be resolved due to dimensions incompatibility: m: "+std::to_string(weightMatrix_.size())+" a: "+std::to_string(array.size())).c_str());
}
std::vector<type> returnVector;

auto iterator=array.begin();
int sum=0;
for (const auto &element : weightMatrix_) {
sum+=static_cast<double>(*iterator)*element;
  iterator++;

if(iterator==array.end()){returnVector.push_back(static_cast<type>(sum>0?+1:-1));iterator=array.begin();sum=0;}
}
return returnVector;
  
    }

   
};//class




} //namespace HN

#endif  //HOPFIELD_NETWORK_HPP