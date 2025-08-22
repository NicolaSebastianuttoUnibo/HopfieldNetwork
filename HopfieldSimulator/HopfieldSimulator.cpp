#include "HopfieldSimulator.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>


//private function
//isHopfieldGoing()

template <typename neurons_type, typename matrix_type> 
 bool HS::HopfieldSimulator<neurons_type,matrix_type>::isHopfieldGoing() const{
for(const auto &element : isStateEvolving_){
  if(element==true){return true;}
}
return false;
}

//public function
///getPatterns()

template <typename neurons_type, typename matrix_type> 
const std::vector<std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>>& HS::HopfieldSimulator<neurons_type,matrix_type>::getPatterns() const {

  return patterns_;
};






//clear()

template<typename neurons_type, typename matrix_type>

void HS::HopfieldSimulator<neurons_type, matrix_type>::clear(const int index){
  hn_.clearEnergy(*patterns_[index]);
}

//corruptPattern()
template <typename neurons_type, typename matrix_type> 

void HS::HopfieldSimulator<neurons_type,matrix_type>::corruptPattern(const size_t index, const float noise) {

  
  if(isStateEvolving_[index]==true){return;}
  if(!check_){return;}

check_=false;
 
  patterns_[index]->reCorrupt(noise);
check_=true;
}

///emplace_pattern()
template<typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::emplace_pattern(const std::string& path, const int a, const int b) {
    patterns_.emplace_back(std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(path,  a,  b));
  
    isStateEvolving_.push_back(false);
cols_=a;rows_=b;
  }
  
template<typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::emplace_pattern(const float noise, const int a, const int b) {
    patterns_.emplace_back(std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(noise,  a,  b));
  
    isStateEvolving_.push_back(false);
cols_=a;rows_=b;
  }



///flipPixelOnPattern()
template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::flipPixelOnPattern(const size_t index, const size_t pos) {
  if(!check_){return;}
 check_=false;
 patterns_[index]->flipNoisyPixel(pos);
 check_=true;

}
///generatePattern()
template <typename neurons_type, typename matrix_type> 

  void HS::HopfieldSimulator<neurons_type,matrix_type>::generatePattern(const float noise, const std::size_t numColumns, const std::size_t numRows){
       patterns_.emplace_back(std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(noise,  numColumns,  numRows));
  
    isStateEvolving_.push_back(false);
cols_=numColumns;rows_=numRows;
  }

///regrid()
template <typename neurons_type, typename matrix_type> 
void HS::HopfieldSimulator<neurons_type,matrix_type>::regrid(size_t numColumns, size_t numRows) {

  if(!check_){return;}
  if(isHopfieldGoing()){return;}
  check_=false;
   cols_=numColumns;
   rows_=numRows;
for(auto &element : patterns_){
  element->regrid(numColumns,  numRows);
}
check_=true;
}

///removePattern()
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


///resolvePattern()
template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::resolvePattern(const int index, float* status) {
 if(!check_){return;}
  
    
    if(patterns_.size()==0){
return;}
 check_=false;
     hn_.resolvePattern(*patterns_[index], status);
    check_=true;
}

///saveFileTraining()

template<typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::saveFileTraining(const std::string& str_buffer) {
    
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

  ///setTraining()
template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::setTraining(const int numColumns, const int numRows, std::vector<matrix_type>& matrix ){
 if(!check_){return;}
    check_=false;
cols_=numColumns;
rows_=numRows;
    const int dim=std::sqrt(matrix.size());
    if(dim*dim!=matrix.size()){
       throw std::logic_error("Dimensione non compatibile");
    }
hn_.setTraining(matrix);
check_=true;
regrid(numColumns,numRows);
  }

///trainNetworkHebb()
template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::trainNetworkHebb(float* status){
 if(!check_){return;}
 check_=false;    
if(isHopfieldGoing()){return;}
    auto function = [](const std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>& csp_ptr){
       
        return csp_ptr->getTrainingPatternVector();
    };

   hn_.trainNetworkWithHebb(patterns_, function,status);

   check_=true;

}


///trainNetworkWithPseudoinverse()
template <typename neurons_type, typename matrix_type> 
  void HS::HopfieldSimulator<neurons_type,matrix_type>::trainNetworkWithPseudoinverse(float* status){
 if(!check_){return;}
 check_=false;    
if(isHopfieldGoing()){return;}
    auto function = [](const std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>& csp_ptr){
       
        return csp_ptr->getTrainingPatternVector();
    };

   hn_.trainNetworkWithPseudoinverse(patterns_, function,status);

   check_=true;

}


///size()
template <typename neurons_type, typename matrix_type> 
 size_t HS::HopfieldSimulator<neurons_type,matrix_type>::size() const{
  return patterns_.size();
}

///checkDimension()

template <typename neurons_type, typename matrix_type> 
 bool HS::HopfieldSimulator<neurons_type,matrix_type>::checkDimension() {
  return rows_*cols_*rows_*cols_==hn_.getTraining().size();
}