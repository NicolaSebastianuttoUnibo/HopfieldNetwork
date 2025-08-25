
#include "../HopfieldSimulator/HopfieldNetwork.hpp"
#include "doctest.h"

TEST_CASE("Hopfield Network") {
  SUBCASE("Two patterns with size 4") {

    HN::HopfieldNetwork<int, double> hn;
    std::vector<std::vector<int>> patterns{{-1, 1, 1, -1}, {1, -1, -1, 1}};
    hn.trainNetworkWithHebb(patterns, [](const auto &p) { return p; });
    std::vector<int> corrupted = {1, -1, 1, -1};

    std::vector<double> matrix = {0,   -0.5, -0.5, 0.5, -0.5, 0,
                                  0.5, -0.5, -0.5, 0.5, 0,    -0.5,
                                  0.5, -0.5, -0.5, 0};
    std::vector<double> matrixFromClass = hn.getTraining();

    CHECK(matrixFromClass.size() == matrix.size());
    for (size_t i = 0; i < matrixFromClass.size(); i++) {
      CAPTURE(i);
      CHECK(matrixFromClass[i] == doctest::Approx(matrix[i]));
    }

    std::vector<int> corr = {1, -1, 1, -1};
    std::vector<int> sol = hn.resolvePattern(corr);

    std::vector<int> vec = {-1, 1, 1, -1};

    CHECK(vec.size() == sol.size());
    for (size_t i = 0; i < vec.size(); i++) {
      CAPTURE(i);
      CHECK(vec[i] == sol[i]);
    }

  } /// fine test
}
