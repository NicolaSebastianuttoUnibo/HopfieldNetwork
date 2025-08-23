

#include "ITrainingPattern.hpp"
#include "math/RandomUtils.hpp"


#include <stdexcept>
#include <algorithm>

template <typename T> 
auto POINTS =   MD::getMathematicalVertex<T>();

//generateRandomPattern()
template <typename T>
  void TP::RandomTrainingPattern<T>::generateRandomPattern(const float randomDensity, const std::size_t numColumns, const std::size_t numRows) {

    if (randomDensity < 0.0f || randomDensity > 1.0f) {
        throw std::invalid_argument("The random density must be in the interval [0.0 , 1.0]");
    }
const size_t N=numColumns*numRows;
    this->pattern_.clear();
this->pattern_.reserve(N);
std::uniform_real_distribution<float> real(0.0f, 1.0f);
std::uniform_int_distribution<int> integer(0, static_cast<int>(POINTS<T>.size() - 1));
auto& generator = RU::getRandomGenerator();

     for (size_t i=0;i<N;i++) {
             int random_index = integer(generator);
             this->pattern_.push_back( POINTS<T>[random_index]);
     


}
  }


///public function of TrainingPattern

//constructor
template <typename T> 

  TP::RandomTrainingPattern<T>::RandomTrainingPattern(const float randomDensity, const std::size_t numColumns, const std::size_t numRows) 
  :  randomDensity_{randomDensity}
   {

    generateRandomPattern(randomDensity, numColumns, numRows);
  }




///regrid()
template <typename T> 
void TP::RandomTrainingPattern<T>::regrid(const std::size_t numColumns, const std::size_t numRows) {

    if (numColumns == 0 || numRows == 0) {
        throw std::invalid_argument("Number of columns and rows must be positive.");
    }
      generateRandomPattern(randomDensity_, numColumns, numRows);
}
