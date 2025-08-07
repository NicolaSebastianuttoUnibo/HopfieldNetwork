#include "TantiPattern.h"
#include "Pattern.h"
#include "quadro.h"
#include "corrotto.h"

void TantiPattern::TantiPattern::creaMatrice(){
    std::vector<std::vector<int>> allmyvectors;
    for(int i=0;i<TantiPattern::TantiPattern::patterns_.size();i++){
        allmyvectors.push_back(TantiPattern::TantiPattern::patterns_[i].getPattern());
    }
TantiPattern::TantiPattern::h_.saveArrays(allmyvectors);


}
void TantiPattern::TantiPattern::disicorrompi(int index){
    // std::vector<int> a;
     TantiPattern::TantiPattern::h_.disicorrompi(TantiPattern::TantiPattern::patterns_[index].getPattern());
    // TantiPattern::TantiPattern::h_.disicorrompi(a);
}
void TantiPattern::TantiPattern::push_back(std::vector<int> vec){

    Pattern::Pattern pattern;
    TantiPattern::TantiPattern::patterns_.push_back(pattern);
}
void TantiPattern::TantiPattern::applicapixelatura(int a, int b){
    for(int i=0;i<TantiPattern::TantiPattern::patterns_.size();i++){
TantiPattern::TantiPattern::patterns_[i].applicaPixelatura(a,b);
    }
}
void TantiPattern::TantiPattern::corrompi(int index){
    TantiPattern::TantiPattern::patterns_[index].Corrompi();
}
