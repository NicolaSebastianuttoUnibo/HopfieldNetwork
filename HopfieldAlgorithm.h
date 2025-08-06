#ifndef HOPFIELD_ALGORITHM_HPP
#define HOPFIELD_ALGORITHM_HPP
#include <vector>
#include <array>
 #include "HopfieldAlgorithm.h"


namespace Hopfield{


struct HopfieldAlgorithm
{

    std::vector<int> matrix;
    void saveArrays(std::vector<std::vector<int>> array);
    std::vector<int> disicorrompi(std::vector<int> array);



};





}

#endif