#ifndef PATTERN_HPP
#define PATTERN_HPP
#include<vector>
#include "corrotto.h"
#include "quadro.h"
namespace Pattern{


struct Pattern
{
std::vector<int> arraydinamico;
Quadro::Quadro q;
Corrotto::Corrotto c;

void applicaPixelatura(int a, int b);
void Corrompi();
std::vector<int> getPattern();


};



}

#endif