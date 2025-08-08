#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <fstream>
#include <iostream>
#include <vector>

double elemento_w(int j, int k, std::vector<std::vector<double>> V) {
  double result{};
  long unsigned int N{V[0].size()};
  for (int i{0}; i < V.size(); i++) {
    if (j == k) {
      double vuoto = {0};
      return vuoto;
    } else {
      result += (V[i][j] * V[i][k]) / N;
    }
  }
  return result;
}
std::vector<double> W(std::vector<std::vector<double>> V) {
  long unsigned int N{V[0].size()};
  std::vector<double> matrix(N * N);

  for (int i{0}; i < N; i++) {
    for (int j{0}; j < N; j++) {
      matrix.at(j * N + i) = elemento_w(i, j, V);
      matrix.at(i * N + j) = matrix[j * N + i];
    }
  }
  return matrix;

  std::ofstream outputfile("matrice_dei_pesi.txt", std::ios::trunc);
  for (int i{0}; i < N; i++) {
    for (int j{0}; j < N; j++) {
      outputfile << matrix.at(i * N + j) << " ";
    }
    outputfile << "\n";
  }
  outputfile.close();
}

TEST_CASE("Elemento") {
  std::vector<std::vector<double>> vec = {{1, 1}, {1, 1}};
  std::vector<std::vector<double>> ko = {{1, 2}, {3, 4}};
  std::vector<std::vector<double>> k1 = {{1, 2, 3, 4}, {5, 3, 4, 2}};
  CHECK(elemento_w(0, 1, ko) == 14 / 2);
  CHECK(elemento_w(1, 2, k1) == 4.5);
  CHECK(elemento_w(1, 0, k1) == 4.25);
  CHECK(elemento_w(0, 0, ko) == 0);
  CHECK(elemento_w(0, 1, vec) == 1);
  CHECK(elemento_w(1, 1, vec) == 0);
  CHECK(elemento_w(1, 0, vec) == 1);
  CHECK(elemento_w(0, 0, vec) == 0);
}
TEST_CASE("W") {
  std::vector<std::vector<double>> vec = {{1, 1}, {1, 1}};
  std::vector<double> vic = {0, 1, 1, 0};
  std::vector<std::vector<double>> x1 = {{-1, 1, 1, -1}, {1, -1, -1, 1}};
  std::vector<double> x2 = {0,    -0.5, -0.5, 0.5,  -0.5, 0,    0.5,  -0.5,
                            -0.5, 0.5,  0,    -0.5, 0.5,  -0.5, -0.5, 0};

  CHECK(W(vec) == vic);
  CHECK(W(x1) == x2);
}
