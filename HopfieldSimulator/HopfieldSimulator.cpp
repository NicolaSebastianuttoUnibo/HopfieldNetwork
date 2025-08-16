#include "HopfieldSimulator.hpp"
#include "TrainingPattern.hpp"
#include "TrainingPattern.hpp"
#include "NoisyPattern.hpp"
#include "EvolvingPattern.hpp"
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <complex>




template <typename neurons_type, typename matrix_type> 
 bool HS::HopfieldSimulator<neurons_type,matrix_type>::isHopfieldGoing() const{
for(const auto &element : isStateEvolving_){
  if(element==true){return true;}
}
return false;
}

#include <utility> // Necessario per std::move e std::forward
#include <memory>  // Necessario per std::make_unique


namespace HS {

// QUESTA È LA CORREZIONE PER emplace_pattern
// NOTA: il 'template<typename... Args>' è posto *prima* del tipo di ritorno.
template<typename neurons_type, typename matrix_type>
// template<typename... Args> // <--- QUESTA RIGA È AL POSTO GIUSTO ORA
void HopfieldSimulator<neurons_type, matrix_type>::emplace_pattern(std::string path, int a, int b) {
    patterns_.emplace_back(std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(path,  a,  b));
  
    isStateEvolving_.push_back(false);

  }
}

// template <typename neurons_type, typename matrix_type> 
// void HS::HopfieldSimulator<neurons_type,matrix_type>::push_back(CSP::CoherenceSetPattern<neurons_type> &pattern) {
//   if(!check_){return;}
//   check_=false;

//   if(patterns_.size()>0){
//   if(!pattern.hasSameDimensionOf(**patterns_.begin())){return;}}
//   rows_=pattern.getRow();
//   cols_=pattern.getCol();
//  *patterns_.push_back((std::move(pattern)));
//  isStateEvolving_.push_back(false);
//  check_=true;
// }



template <typename neurons_type, typename matrix_type> 
 size_t HS::HopfieldSimulator<neurons_type,matrix_type>::size() const{
  return patterns_.size();
}

template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::regrid(size_t numColumns, size_t numRows) {

  if(!check_){return;}
  if(isHopfieldGoing()){return;}
  check_=false;
   cols_=numColumns;
   rows_=numRows;
  //  std::cout<<cols_<<"\n";
  //  std::cout<<rows_<<"\n";
  //     std::cout<<hn_.getTraining().size()<<"\n";
  //     std::cout<<"______"<<"\n";

 
for(auto &element : patterns_){
  element->regrid(numColumns,  numRows);
}


check_=true;
}

template <typename neurons_type, typename matrix_type> 

void HS::HopfieldSimulator<neurons_type,matrix_type>::corruptPattern(const size_t index, const float noise) {

  
  if(isStateEvolving_[index]==true){return;}
  if(!check_){return;}

check_=false;
 
  patterns_[index]->reCorrupt(noise);
check_=true;
}

template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::flipPixelOnPattern(const size_t index, const size_t pos) {
  if(!check_){return;}
 check_=false;
 patterns_[index]->flipNoisyPixel(pos);
 check_=true;

}


template <typename neurons_type, typename matrix_type> 
const std::vector<std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>>& HS::HopfieldSimulator<neurons_type,matrix_type>::getPatterns() const {

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
  void HS::HopfieldSimulator<neurons_type,matrix_type>::trainNetworkHebb(float* status){
 if(!check_){return;}
 check_=false;    
if(isHopfieldGoing()){return;}
    auto function = [](const std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>& csp_ptr){
       
        return csp_ptr->getTrainingPattern().getPattern();
    };

   hn_.trainNetworkHebb(patterns_, function,status);

   check_=true;

}

template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::trainNetworkWithPseudoinverse(float* status){
 if(!check_){return;}
 check_=false;    
if(isHopfieldGoing()){return;}
    auto function = [](const std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>& csp_ptr){
       
        return csp_ptr->getTrainingPattern().getPattern();
    };

   hn_.trainNetworkWithPseudoinverse(patterns_, function,status);

   check_=true;

}






template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::resolvePattern(const int index, float* status) {
 if(!check_){return;}
  
    // std::cout<<"here\n";
    
    if(patterns_.size()==0){    //std::cout<<"here2\n";
return;}//std::cout<<"here3\n";
    const int dim = (*(patterns_[0])).getTrainingPatternVector().size();
    if(!check_||hn_.getTraining().size()!=dim*dim){std::cout<<"here4\n";return;}
 check_=false;
// std::cout<<"here5\n";
     hn_.resolvePattern(*patterns_[index], status);
    check_=true;
}

  

template <typename neurons_type, typename matrix_type> 
size_t HS::HopfieldSimulator<neurons_type,matrix_type>::size(){
return patterns_.size();
}
  
template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::setTraining(const int numColumns, const int numRows, std::vector<matrix_type>& matrix ){
 if(!check_){return;}
    check_=false;
cols_=numColumns;
rows_=numRows;
    const int dim=std::sqrt(matrix.size());
    assert(dim*dim==matrix.size());
hn_.setTraining(matrix);
check_=true;
regrid(numColumns,numRows);
  }

template<typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::saveFileTraining(char* str_buffer) {
    
    const int numColumns = cols_;
    const int numRows = rows_;

    const std::vector<matrix_type>& matrix = hn_.getTraining();

    try {

      std::filesystem::path filePath(str_buffer);
      filePath.replace_extension(".training"); 
      std::filesystem::path directoryPath = filePath.parent_path();

        if (!directoryPath.empty() && !std::filesystem::exists(directoryPath)) {
            if (!std::filesystem::create_directories(directoryPath)) {
               throw std::logic_error("Problemi con la creazione della cartella: " + directoryPath.string());
            }
        }
        
       
        std::ofstream outFile(filePath);

        if (!outFile.is_open()) {
            throw std::logic_error("Impossibile aprire il file per la scrittura: " + filePath.string());
        }

      
        outFile << numRows << " " << numColumns << "\n";

        if (!matrix.empty()) {
            
            for (const auto& element : matrix) { 
                    outFile << element << " ";
            }
        }

        outFile.close(); 

    } catch (const std::filesystem::filesystem_error& e) {
        throw std::logic_error("Errore del filesystem: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        throw;
    }
}