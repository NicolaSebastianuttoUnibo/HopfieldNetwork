#include "TantiPattern.h"
#include "Pattern.h"
#include "corrotto.h"
#include "quadro.h"
#include <iostream>
void TantiPattern::TantiPattern::creaMatrice() {
  std::vector<std::vector<int>> allmyvectors;
  for (int i = 0; i < TantiPattern::TantiPattern::patterns_.size(); i++) {
    allmyvectors.push_back(
        TantiPattern::TantiPattern::patterns_[i].getPatternQ());
  }
  TantiPattern::TantiPattern::h_.saveArrays(allmyvectors);
}
void TantiPattern::TantiPattern::disicorrompi(int index) {
  // std::vector<int> a;
  TantiPattern::TantiPattern::h_.disicorrompi(
      TantiPattern::TantiPattern::patterns_[index].getPatternD());
  // TantiPattern::TantiPattern::h_.disicorrompi(a);
}

void TantiPattern::TantiPattern::push_back(Pattern::Pattern &pattern) {

  TantiPattern::TantiPattern::patterns_.push_back(pattern);
}
int TantiPattern::TantiPattern::size() {
  return TantiPattern::TantiPattern::patterns_.size();
}
void TantiPattern::TantiPattern::applicapixelatura(int a, int b) {
  for (int i = 0; i < TantiPattern::TantiPattern::patterns_.size(); i++) {
    TantiPattern::TantiPattern::patterns_[i].applicaPixelatura(a, b);
  }
}
void TantiPattern::TantiPattern::corrompi(int index) {
  TantiPattern::TantiPattern::patterns_[index].Corrompi();
  // std::cout<<"ciao";
}
void TantiPattern::TantiPattern::corrompiPixel(int index, int pos) {
  TantiPattern::TantiPattern::patterns_[index].CorrompiPixel(pos);
  // std::cout<<"ciao";
}

std::vector<Pattern::Pattern> &TantiPattern::TantiPattern::getPatterns() {
  return TantiPattern::TantiPattern::patterns_;
};

void TantiPattern::TantiPattern::removePattern(int index) {
  if (index >= 0 && index < TantiPattern::TantiPattern::patterns_.size()) {
    TantiPattern::TantiPattern::patterns_.erase(
        TantiPattern::TantiPattern::patterns_.begin() + index);
  }
}
