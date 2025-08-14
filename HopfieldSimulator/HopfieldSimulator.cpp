#include "HopfieldSimulator.hpp"
#include "TrainingPattern.hpp"
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include <stdexcept>
#include <cassert>

template <typename neurons_type, typename matrix_type> 
 bool HS::HopfieldSimulator<neurons_type,matrix_type>::isHopfieldGoing() const{
for(const auto &element : isStateEvolving_){
  if(element==true){return true;}
}
return false;
}



template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::push_back(CSP::CoherenceSetPattern<neurons_type> &pattern) {
  if(!check_){return;}
  check_=false;

  if(patterns_.size()>0){
  if(!pattern.hasSameDimensionOf(*patterns_.begin())){return;}}
 patterns_.push_back((std::move(pattern)));
 isStateEvolving_.push_back(false);
 check_=true;
}



template <typename neurons_type, typename matrix_type> 
 size_t HS::HopfieldSimulator<neurons_type,matrix_type>::size() const{
  return patterns_.size();
}

template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::regrid(size_t numColumns, size_t numRows) {

  if(!check_){return;}
  check_=false;
  if(isHopfieldGoing()){return;}
for(auto &element : patterns_){
  element.regrid(numColumns,  numRows);
}
check_=true;
}

template <typename neurons_type, typename matrix_type> 

void HS::HopfieldSimulator<neurons_type,matrix_type>::corruptPattern(const size_t index, const float noise) {
  if(isStateEvolving_[index]==true){return;}
  if(!check_){return;}
check_=false;
  patterns_[index].reCorrupt(noise);
check_=true;
}

template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::flipPixelOnPattern(const size_t index, const size_t pos) {
  if(!check_){return;}
 check_=false;
 patterns_[index].flipNoisyPixel(pos);
 check_=true;

}


template <typename neurons_type, typename matrix_type> 
const std::vector<CSP::CoherenceSetPattern<neurons_type>>& HS::HopfieldSimulator<neurons_type,matrix_type>::getPatterns() const {

  return patterns_;
};


template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::removePattern(const size_t index) {
 if(!check_){return;}
 check_=false;

  if (index < patterns_.size()) {
    patterns_.erase(patterns_.begin() + index);
    isStateEvolving_.erase(isStateEvolving_.begin() + index);
  }
  else{
  throw std::runtime_error("The pattern you want to delete doesn't exist");

  }
  check_=true;
}


template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::trainNetwork(float* status){
 if(!check_){return;}
 check_=false;    
if(isHopfieldGoing()){return;}
auto function=[](const CSP::CoherenceSetPattern<neurons_type> &csp) {
    return csp.getTrainingPattern().getPattern();
};

   hn_.trainNetwork(patterns_, function,status);

   check_=true;

}


template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::resolvePattern(const int index, float* status) {
     if(patterns_.size()==0){return;}
    const int dim = patterns_[0].getTrainingPatternVector().size();
    if(!check_||hn_.getTraining().size()!=dim*dim){return;}
 check_=false;
    
    //  auto& current_evolving_vector = patterns_[index].getEvolvingPattern().getPatternPointer();

    // auto newVector = hn_.resolvePattern(current_evolving_vector);
// hn_.resolvePattern(&current_evolving_vector);
    // patterns_[index].updateEvolvingState(newVector);
     hn_.resolvePattern(patterns_[index], status);
    check_=true;
}

  

template <typename neurons_type, typename matrix_type> 
size_t HS::HopfieldSimulator<neurons_type,matrix_type>::size(){
return patterns_.size();
}
  
