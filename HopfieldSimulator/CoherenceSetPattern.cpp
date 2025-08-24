
#include "CoherenceSetPattern.hpp"
#include <stdexcept> 


///private function
//getEvolvingPattern()
template <typename T> 
   EP::EvolvingPattern<T>& CSP::CoherenceSetPattern<T>::getEvolvingPattern()  noexcept{return ep_;}

///public function
//constructor
template <typename T> 
    CSP::CoherenceSetPattern<T>::CoherenceSetPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows) : tp_(std::make_unique<TP::ImageTrainingPattern<T>>(path, numColumns, numRows)), np_(tp_->getPattern()), ep_(np_.getPattern()),cols_{numColumns}, rows_{numRows}  { }


    // second constructor
template <typename T> 
    CSP::CoherenceSetPattern<T>::CoherenceSetPattern(const float noise, const std::size_t numColumns, const std::size_t numRows) : tp_(std::make_unique<TP::RandomTrainingPattern<T>>(noise, numColumns, numRows)), np_(tp_->getPattern()), ep_(np_.getPattern()),cols_{numColumns}, rows_{numRows}  { }

    ///getTrainingPatternVector()
template <typename T> 
  const std::vector<T>& CSP::CoherenceSetPattern<T>::getTrainingPatternVector() const noexcept {
return tp_->getPattern();
  }
  
  ///getNoisyPatternVector()
template <typename T> 
  const std::vector<T>& CSP::CoherenceSetPattern<T>::getNoisyPatternVector() const noexcept{
return np_.getPattern();
  }
///getEvolvingPatternVector()
template <typename T> 
  const std::vector<T>& CSP::CoherenceSetPattern<T>::getEvolvingPatternVector() const noexcept{
return ep_.getPattern();
  }


  //getEnergy()
  template <typename T> 
  const std::vector<float>& CSP::CoherenceSetPattern<T>::getEnergy() const noexcept{
return ep_.getEnergy();
  }
//getRow()
template <typename T> 
     size_t CSP::CoherenceSetPattern<T>::getRow() const noexcept{
      return rows_;
    }
    ///getCol()
template <typename T> 
   size_t CSP::CoherenceSetPattern<T>::getCol() const noexcept{
      return cols_;
  }

///hasSameDimensionOf()
template <typename T> 
  bool CSP::CoherenceSetPattern<T>::hasSameDimensionOf(CoherenceSetPattern& other)const{
     return other.cols_==cols_&&other.rows_==rows_;
   
  }

///hasTheseDimensions()
template <typename T> 
  bool CSP::CoherenceSetPattern<T>::hasTheseDimensions(const size_t col, const size_t row)const{
    return col==cols_&&row==rows_;
  }

///flipNoisyPixel()
template <typename T> 
  void CSP::CoherenceSetPattern<T>::flipNoisyPixel(std::size_t pos){
    np_.cyclePixelState(pos);
    ep_.getPattern()=np_.getPattern();

  }

///reCorrupt()

template <typename T> 
  void CSP::CoherenceSetPattern<T>::reCorrupt(const float noise){
np_=NP::NoisyPattern(tp_->getPattern(),noise);
ep_.getPattern()=np_.getPattern();
  }

//regrid()
template <typename T> 
  void CSP::CoherenceSetPattern<T>::regrid(const std::size_t numColumns, const std::size_t numRows){
    rows_=numRows;
    cols_=numColumns;
    tp_->regrid(numColumns,numRows);
reCorrupt();
  }

///updateEvolvingState()

template <typename T> 
  void CSP::CoherenceSetPattern<T>::updateEvolvingState(const std::vector<T>& newPattern){
if(newPattern.size()!=ep_.getPattern().size()){
          throw std::invalid_argument("You cannot change the dimension of the evolving state");
}
    ep_.getPattern()=(newPattern);

  }
  
  













