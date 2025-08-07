#ifndef TANTI_PATTERN_HPP
#define TANTI_PATTERN_HPP
#include "Pattern.h"
#include "HopfieldAlgorithm.h"
#include <vector>

namespace TantiPattern{


class  TantiPattern
{

    
std::vector<Pattern::Pattern> patterns_;
Hopfield::HopfieldAlgorithm h_;
public:
TantiPattern()=default;
void creaMatrice();
void disicorrompi(int index);
void push_back(Pattern::Pattern pattern);
void applicapixelatura(int a, int b);
void corrompi(int index);
void corrompiPixel(int index, int pos);
int size();
void removePattern(int index);
std::vector<Pattern::Pattern> getPatterns();


};



}

#endif