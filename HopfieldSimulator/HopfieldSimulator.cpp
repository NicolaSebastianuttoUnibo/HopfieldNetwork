#include "HopfieldSimulator.hpp"
#include "TrainingPattern.hpp"
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include <stdexcept>


 bool HS::HopfieldSimulator::isHopfieldGoing() const{
for(const auto &element : isStateEvolving_){
  if(element==true){return true;}
}
return false;
}




void HS::HopfieldSimulator::push_back(CSP::CoherenceSetPattern &pattern) {

 patterns_.push_back(pattern);
 isStateEvolving_.push_back(false);
}

 size_t HS::HopfieldSimulator::size() const{
  return patterns_.size();
}
void HS::HopfieldSimulator::regrid(size_t numColumns, size_t numRows) {
if(isHopfieldGoing()){return;}
for(auto &element : patterns_){
  element.regrid(numColumns,  numRows);
}
}
void HS::HopfieldSimulator::corruptPattern(const size_t index) {
  if(isStateEvolving_[index]==true){return;}
  patterns_[index].reCorrupt();

}
void HS::HopfieldSimulator::flipPixelOnPattern(const size_t index, const size_t pos) {
  if(isStateEvolving_[index]==true){return;}
 patterns_[index].flipNoisyPixel(pos);

}

const std::vector<CSP::CoherenceSetPattern>& HS::HopfieldSimulator::getPatterns() const {
return patterns_;
};

void HS::HopfieldSimulator::removePattern(const size_t index) {
 // if(isStateEvolving_[index]==true){return;}

  if (index < patterns_.size()) {
    patterns_.erase(patterns_.begin() + index);
    isStateEvolving_.erase(isStateEvolving_.begin() + index);
  }
  else{
  throw std::runtime_error("The pattern you want to delete doesn't exist");

  }
}

  void HS::HopfieldSimulator::trainNetwork(){
    
if(isHopfieldGoing()){return;}
auto function=[](const CSP::CoherenceSetPattern &csp) {
    return csp.getEvolvingPattern().getPattern();
};

   hn_.trainNetwork(patterns_, function );
}

void HS::HopfieldSimulator::resolvePattern(const int index){
  std::vector<int8_t> aaa=patterns_[index].getEvolvingPattern().getPattern();
   auto newVector=hn_.resolvePattern(aaa);
   patterns_[index].updateEvolvingState(newVector);
}