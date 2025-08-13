
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include "CoherenceSetPattern.hpp"
 #include <stdexcept> 


template <typename T> 
    CSP::CoherenceSetPattern<T>::CoherenceSetPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows) : tp_(path, numColumns, numRows), np_(tp_.getPattern()), ep_(np_.getPattern()),cols_{numColumns}, rows_{numRows}  { }

template <typename T> 
  void CSP::CoherenceSetPattern<T>::regrid(const std::size_t numColumns, const std::size_t numRows){
    rows_=numRows;
    cols_=numColumns;
    tp_.regrid(numColumns,numRows);
reCorrupt();
  }

template <typename T> 
  void CSP::CoherenceSetPattern<T>::reCorrupt(const float noise){
np_=NP::NoisyPattern(tp_.getPattern(),noise);
ep_.getPattern()=np_.getPattern();
  }

template <typename T> 
  void CSP::CoherenceSetPattern<T>::flipNoisyPixel(std::size_t pos){
    np_.flipPixel(pos);
    ep_.getPattern()=np_.getPattern();

  }

template <typename T> 
  void CSP::CoherenceSetPattern<T>::updateEvolvingState(const std::vector<T>& newPattern){
if(newPattern.size()!=ep_.getPattern().size()){
          throw std::invalid_argument("You cannot change the dimension of the evolving state");
}
    ep_.getPattern()=(newPattern);

  }
  
  
template <typename T> 
  const TP::TrainingPattern<T>& CSP::CoherenceSetPattern<T>::getTrainingPattern() const noexcept {
return tp_;
  } 

template <typename T> 
  const NP::NoisyPattern<T>& CSP::CoherenceSetPattern<T>::getNoisyPattern() const noexcept{
return np_;
  }

template <typename T> 
  const EP::EvolvingPattern<T>& CSP::CoherenceSetPattern<T>::getEvolvingPattern() const noexcept{
return ep_;
  }




template <typename T> 
  const std::vector<T>& CSP::CoherenceSetPattern<T>::getTrainingPatternVector() const noexcept {
return tp_.getPattern();
  }
  
template <typename T> 
  const std::vector<T>& CSP::CoherenceSetPattern<T>::getNoisyPatternVector() const noexcept{
return np_.getPattern();
  }

template <typename T> 
  const std::vector<T>& CSP::CoherenceSetPattern<T>::getEvolvingPatternVector() const noexcept{
return ep_.getPattern();
  }

  template <typename T> 
  const std::vector<float>& CSP::CoherenceSetPattern<T>::getEnergy() const noexcept{
return ep_.getEnergy();
  }

template <typename T> 
  bool CSP::CoherenceSetPattern<T>::hasSameDimensionOf(CoherenceSetPattern& other){
     return other.cols_==cols_&&other.rows_==rows_;
   
  }


  