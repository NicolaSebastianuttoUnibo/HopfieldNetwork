
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include "CoherenceSetPattern.hpp"
 #include <stdexcept> 


    CSP::CoherenceSetPattern::CoherenceSetPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows) : tp_(path, numColumns, numRows), np_(tp_.getPattern()), ep_(np_.getPattern()) { }


  void CSP::CoherenceSetPattern::regrid(const std::size_t numColumns, const std::size_t numRows){
    tp_.regrid(numColumns,numRows);
reCorrupt();
  }
  void CSP::CoherenceSetPattern::reCorrupt(const float noise){
np_=NP::NoisyPattern(tp_.getPattern(),noise);
ep_.updatePattern(np_.getPattern());
  }
  void CSP::CoherenceSetPattern::flipNoisyPixel(std::size_t pos){

    np_.flipPixel(pos);
    ep_.updatePattern(np_.getPattern());

  }

  void CSP::CoherenceSetPattern::updateEvolvingState(const std::vector<int8_t>& newPattern){
if(newPattern.size()!=ep_.getPattern().size()){
          throw std::invalid_argument("You cannot change the dimension of the evolving state");
}
    ep_.updatePattern(newPattern);

  }
  
  
  const TP::TrainingPattern& CSP::CoherenceSetPattern::getTrainingPattern() const noexcept {
return tp_;
  } 
  const NP::NoisyPattern& CSP::CoherenceSetPattern::getNoisyPattern() const noexcept{
return np_;
  }
  const EP::EvolvingPattern& CSP::CoherenceSetPattern::getEvolvingPattern() const noexcept{
return ep_;
  }