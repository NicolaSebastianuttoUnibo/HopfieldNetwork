#include "Pattern.h"
#include "corrotto.h"
#include "quadro.h"
#include <iostream>
void Pattern::Pattern::applicaPixelatura(int a, int b) {
  Pattern::Pattern::q_.updatePattern(a, b);
  //  Quadro::Quadro qqq;
  //   Corrotto::Corrotto(Pattern::Pattern::q.getPattern());
  Pattern::Pattern::Corrompi();
}

void Pattern::Pattern::Corrompi() {
  Pattern::Pattern::c_ = Corrotto::Corrotto(Pattern::Pattern::q_.getPattern());
  Pattern::Pattern::arraydinamico_ = c_.getPattern();
}

void Pattern::Pattern::CorrompiPixel(int pos) { c_.CorrompiPixel(pos); }

std::vector<int> &Pattern::Pattern::getPatternQ() {
  return Pattern::Pattern::q_.getPattern();
}

std::vector<int> &Pattern::Pattern::getPatternC() {
  return Pattern::Pattern::c_.getPattern();
}

std::vector<int> &Pattern::Pattern::getPatternD() {
  return Pattern::Pattern::arraydinamico_;
}

Pattern::Pattern::Pattern(std::string &str, int a, int b) {

  Pattern::Pattern::q_ = Quadro::Quadro(str, a, b);
  Pattern::Pattern::Corrompi();
}