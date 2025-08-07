#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

double elemento(std::vector<int> V, std::vector<int> U) {
  double result;
  for (int i = 0; i < V.size(); i++) {
    result += V.at(i) * U.at(i);
  }
  return result;
}


std::vector<std::vector<int>> W(std::vector<std::vector<int>> V) {
    if(V.size()==0){
        return V;
    }
  int i=V[0].size();
  int j=i;
  std::vector<std::vector<int>> result;
  for(int k{0};k < i;k++) {
    std::vector<int> c;
    for( int  m{0};m < j;m++) {
      if (m == k) {
        c.push_back(0);
      } else {

        c.push_back(elemento(V[k], V[m]));//MANCA N E P
      }
    }
    result.push_back(c);
  }
  return result;

  /* std::ofstream outputfile("matrice_dei_pesi");
   outputfile << result;
   outputfile.close();*/
}
//TEST_CASE("Elemento") { CHECK(elemento({1, 2}, {3, 4}) == 11); }


TEST_CASE("W") {
    std::vector<std::vector<int>> vec= {{1, 1}, {1, 1}};
    std::vector<std::vector<int>> vic= {{0, 2}, {2, 0} };
  CHECK(W(vec) == vic);
}
