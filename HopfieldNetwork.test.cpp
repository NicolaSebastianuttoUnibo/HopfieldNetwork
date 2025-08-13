#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "HopfieldSimulator/HopfieldNetwork.hpp"
#include "doctest.h"
#include <iostream>

TEST_CASE("Testing the class handling a floating point data sample") {
  SUBCASE("Checking first one") {

    HN::HopfieldNetwork<int, double> hn;
    std::vector<std::vector<int>> patterns{{-1, 1, 1, -1}, {1, -1, -1, 1}};
    hn.trainNetwork(patterns, [](const auto &p) { return p; });
    std::vector<int> corrupted = {1, -1, 1, -1};

    std::vector<double> matrix = {0,   -0.5, -0.5, 0.5, -0.5, 0,
                                  0.5, -0.5, -0.5, 0.5, 0,    -0.5,
                                  0.5, -0.5, -0.5, 0};
    std::vector<double> matrixFromClass = hn.getTraining();

    CHECK(matrixFromClass.size() == matrix.size());
    for (size_t i = 0; i < matrixFromClass.size(); i++) {
      CAPTURE(i);
      CHECK(matrixFromClass[i] ==
            doctest::Approx(
                matrix[i])); // Usare Approx è buona pratica con i double
    }

    std::vector<int> corr = {1, -1, 1, -1};
    std::vector<int> sol = hn.resolvePattern(corr);

    std::vector<int> vec = {-1, 1, 1, -1};

    CHECK(vec.size() == sol.size());
    for (size_t i = 0; i < vec.size(); i++) {
      CAPTURE(i);
      CHECK(vec[i] == sol[i]);
    }
  }

  SUBCASE("secondo test") {

    HN::HopfieldNetwork<int, double> hn;

    // Pattern di addestramento
    std::vector<std::vector<int>> patterns{{-1, -1, -1, -1}, {1, 1, -1, -1}};

    hn.trainNetwork(patterns, [](const auto &p) { return p; });

    // Pattern corrotto da testare (basato sul primo pattern con un bit
    // flippato)
    std::vector<int> corruptedPattern = {1, -1, -1, -1};

    // Esegui la ricostruzione del pattern
    std::vector<int> resolvedPattern = hn.resolvePattern(corruptedPattern);

    // La soluzione attesa è il pattern originale
    std::vector<int> expectedSolution = {-1, -1, -1, -1};

    // Verifica che il pattern risolto corrisponda all'originale
    CHECK(resolvedPattern.size() == expectedSolution.size());
    for (size_t i = 0; i < expectedSolution.size(); ++i) {
      CAPTURE(i);
      CHECK(resolvedPattern[i] == expectedSolution[i]);
    }
  }

  SUBCASE("terzo test") {

    HN::HopfieldNetwork<int, double> hn;

    // Pattern di addestramento
    std::vector<std::vector<int>> patterns{{-1, 1,  -1, 1,  -1, 1,  -1, 1, -1,
                                            1,  -1, -1, 1,  -1, 1,  -1, 1, -1,
                                            -1, -1, -1, -1, -1, -1, 1}};

    hn.trainNetwork(patterns, [](const auto &p) { return p; });

    // Pattern corrotto da testare (basato sul primo pattern con un bit
    // flippato)
    std::vector<int> corruptedPattern = {1,  1,  -1, 1,  -1, 1,  -1, 1, -1,
                                         1,  -1, -1, 1,  -1, 1,  -1, 1, -1,
                                         -1, -1, -1, -1, -1, -1, 1};

    // Esegui la ricostruzione del pattern
    std::vector<int> resolvedPattern = hn.resolvePattern(corruptedPattern);

    // La soluzione attesa è il pattern originale
    std::vector<int> expectedSolution = {-1, 1,  -1, 1,  -1, 1,  -1, 1, -1,
                                         1,  -1, -1, 1,  -1, 1,  -1, 1, -1,
                                         -1, -1, -1, -1, -1, -1, 1};

    // Verifica che il pattern risolto corrisponda all'originale
    CHECK(resolvedPattern.size() == expectedSolution.size());
    for (size_t i = 0; i < expectedSolution.size(); ++i) {
      CAPTURE(i);
      CHECK(resolvedPattern[i] == expectedSolution[i]);
    }
  }

  SUBCASE("quarto test") {
    HN::HopfieldNetwork<int, double> hn;

    // Pattern di addestramento
    std::vector<std::vector<int>> patterns{
        {1,  -1, 1,  -1, 1,  -1, 1,  -1, -1, -1, 1,  -1, -1, 1,  -1, 1,  1,  -1,
         1,  -1, 1,  1,  -1, -1, -1, 1,  -1, -1, -1, 1,  -1, 1,  1,  -1, 1,  -1,
         1,  1,  -1, 1,  1,  1,  1,  -1, -1, 1,  1,  1,  -1, -1, 1,  1,  1,  -1,
         -1, 1,  1,  -1, 1,  1,  1,  -1, 1,  1,  -1, -1, -1, 1,  -1, 1,  -1, -1,
         -1, -1, 1,  -1, -1, 1,  1,  -1, 1,  -1, 1,  -1, 1,  1,  1,  1,  -1, 1,
         1,  1,  -1, 1,  -1, 1,  1,  -1, 1,  1,  1,  -1, -1, 1,  1,  -1, 1,  -1,
         -1, -1, 1,  -1, 1,  -1, -1, -1, -1, 1,  1,  -1, -1, 1,  -1, 1,  1,  1,
         -1, -1, -1, 1,  -1, -1, 1,  1,  1,  -1, 1,  -1, 1,  1,  -1, -1, -1, -1,
         -1, 1,  -1, 1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, -1, 1,  -1, 1,  -1,
         -1, -1, 1,  -1, 1,  -1, -1, 1,  1,  -1, 1,  1,  -1, 1,  -1, -1, -1, 1,
         1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, 1,  1,  1,  -1, 1,  1,  -1, 1,
         1,  -1, -1, -1, 1,  -1, 1,  1,  -1, 1,  -1, -1, -1, 1,  1,  -1, 1,  -1,
         -1, 1,  -1, 1,  1,  -1, 1,  1,  1,  1,  -1, -1, -1, -1, -1, 1,  1,  1,
         1,  1,  1,  1,  1,  1,  -1, -1, 1,  1,  1,  1,  -1, -1, -1, -1, 1,  -1,
         1,  1,  1,  1},
        {-1, 1,  -1, 1,  -1, 1,  -1, 1,  1,  1,  1,  1,  1,  -1, -1, 1,  -1, 1,
         1,  1,  1,  1,  -1, 1,  -1, -1, -1, -1, -1, 1,  -1, -1, -1, 1,  -1, -1,
         1,  1,  1,  -1, -1, -1, -1, 1,  1,  -1, 1,  -1, -1, -1, 1,  1,  -1, -1,
         1,  -1, -1, 1,  -1, -1, -1, 1,  1,  1,  1,  -1, 1,  1,  1,  1,  -1, -1,
         -1, -1, 1,  -1, 1,  -1, -1, 1,  -1, 1,  -1, 1,  -1, -1, 1,  -1, -1, 1,
         1,  -1, 1,  1,  1,  -1, -1, -1, 1,  -1, -1, -1, -1, -1, -1, -1, 1,  -1,
         -1, -1, 1,  -1, 1,  1,  -1, -1, 1,  -1, 1,  -1, 1,  -1, -1, 1,  -1, -1,
         1,  1,  -1, -1, 1,  -1, -1, 1,  1,  -1, -1, 1,  1,  -1, 1,  -1, 1,  -1,
         -1, -1, -1, 1,  1,  -1, -1, -1, 1,  1,  -1, 1,  1,  1,  1,  -1, -1, -1,
         -1, -1, 1,  -1, 1,  1,  1,  -1, -1, -1, 1,  -1, -1, 1,  1,  1,  -1, -1,
         1,  -1, -1, -1, -1, -1, 1,  -1, 1,  1,  -1, 1,  1,  -1, 1,  -1, -1, 1,
         1,  -1, 1,  1,  1,  -1, 1,  -1, -1, -1, 1,  1,  1,  -1, 1,  -1, 1,  -1,
         -1, 1,  -1, -1, -1, -1, -1, 1,  1,  1,  -1, -1, -1, 1,  1,  1,  1,  1,
         1,  -1, -1, 1,  -1, 1,  1,  -1, 1,  1,  -1, 1,  1,  1,  -1, -1, -1, 1,
         1,  1,  -1, -1},
        {1,  -1, -1, 1,  1,  1,  1,  -1, -1, 1,  -1, 1,  -1, -1, -1, 1,  1,  1,
         1,  1,  -1, 1,  -1, 1,  1,  -1, -1, -1, 1,  1,  1,  -1, 1,  1,  -1, 1,
         1,  -1, -1, 1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, 1,  -1, 1,  1,  -1,
         1,  -1, -1, -1, -1, -1, 1,  1,  -1, -1, -1, -1, 1,  -1, -1, 1,  1,  1,
         -1, -1, 1,  1,  -1, -1, 1,  -1, -1, -1, -1, 1,  -1, -1, 1,  -1, -1, -1,
         -1, -1, 1,  1,  1,  1,  -1, -1, -1, 1,  -1, -1, 1,  1,  1,  -1, -1, -1,
         1,  -1, -1, 1,  1,  -1, 1,  -1, -1, 1,  1,  -1, -1, 1,  -1, -1, -1, 1,
         1,  -1, -1, -1, -1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  -1, -1, 1,  1,
         1,  -1, -1, -1, -1, 1,  1,  1,  1,  1,  -1, 1,  1,  -1, 1,  -1, -1, 1,
         -1, 1,  1,  -1, -1, -1, 1,  -1, -1, 1,  1,  1,  -1, 1,  1,  1,  1,  1,
         -1, -1, -1, 1,  1,  -1, 1,  -1, -1, -1, -1, 1,  1,  1,  -1, 1,  -1, -1,
         1,  1,  -1, 1,  1,  -1, -1, -1, 1,  1,  -1, 1,  -1, -1, -1, 1,  1,  -1,
         1,  1,  -1, -1, 1,  1,  -1, -1, 1,  1,  1,  1,  -1, 1,  -1, 1,  -1, -1,
         -1, 1,  1,  -1, 1,  -1, 1,  -1, 1,  -1, -1, -1, -1, 1,  1,  -1, 1,  -1,
         1,  1,  1,  1},
        {1,  -1, 1,  1,  1,  -1, 1,  1,  -1, -1, 1,  -1, -1, 1,  1,  -1, -1, -1,
         1,  -1, -1, -1, 1,  -1, 1,  1,  1,  1,  -1, -1, 1,  -1, 1,  -1, -1, -1,
         1,  1,  -1, -1, -1, -1, -1, -1, 1,  1,  1,  -1, -1, -1, -1, 1,  -1, -1,
         -1, -1, 1,  -1, -1, 1,  1,  1,  1,  -1, 1,  1,  1,  -1, 1,  1,  -1, -1,
         -1, 1,  -1, 1,  -1, -1, 1,  -1, 1,  1,  -1, -1, -1, 1,  -1, 1,  1,  1,
         1,  -1, 1,  -1, -1, -1, -1, -1, 1,  -1, -1, -1, 1,  -1, 1,  1,  1,  -1,
         -1, 1,  -1, 1,  -1, 1,  -1, -1, 1,  -1, 1,  1,  -1, 1,  -1, 1,  -1, -1,
         1,  1,  -1, 1,  1,  -1, -1, -1, 1,  -1, -1, 1,  -1, 1,  1,  1,  -1, -1,
         1,  -1, -1, 1,  1,  -1, 1,  -1, 1,  -1, -1, 1,  -1, -1, -1, -1, -1, -1,
         1,  1,  1,  1,  1,  1,  -1, 1,  1,  -1, 1,  -1, -1, 1,  1,  -1, 1,  1,
         1,  -1, 1,  1,  1,  -1, -1, 1,  1,  1,  1,  -1, 1,  -1, -1, -1, 1,  1,
         -1, -1, -1, -1, -1, 1,  -1, -1, -1, 1,  -1, -1, -1, -1, 1,  1,  1,  -1,
         1,  1,  1,  -1, 1,  1,  1,  1,  1,  -1, 1,  1,  -1, 1,  -1, 1,  1,  -1,
         1,  1,  -1, -1, 1,  1,  -1, -1, 1,  1,  -1, 1,  1,  -1, -1, 1,  1,  -1,
         -1, -1, 1,  1}};

    hn.trainNetwork(patterns, [](const auto &p) { return p; });

    // Pattern corrotto da testare (basato sul primo pattern con un bit
    // flippato)
    std::vector<int> corruptedPattern = {
        -1, 1,  1,  1,  1,  -1, 1,  -1, -1, -1, 1,  -1, -1, 1,  -1, 1,  1,  1,
        1,  -1, 1,  1,  -1, -1, 1,  1,  -1, -1, -1, 1,  1,  1,  1,  1,  1,  -1,
        1,  1,  -1, 1,  1,  1,  1,  -1, -1, 1,  1,  1,  -1, -1, 1,  1,  1,  -1,
        -1, 1,  1,  -1, 1,  1,  1,  -1, 1,  1,  -1, -1, -1, 1,  -1, 1,  -1, -1,
        -1, -1, 1,  -1, -1, 1,  1,  -1, 1,  -1, 1,  -1, 1,  1,  1,  1,  -1, 1,
        1,  1,  1,  1,  -1, 1,  1,  -1, 1,  1,  1,  -1, -1, 1,  1,  -1, 1,  -1,
        -1, -1, 1,  1,  1,  -1, -1, -1, -1, 1,  1,  -1, -1, 1,  -1, 1,  1,  1,
        -1, -1, -1, 1,  -1, -1, 1,  1,  1,  -1, 1,  -1, 1,  1,  -1, 1,  -1, -1,
        -1, 1,  -1, 1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, -1, 1,  -1, 1,  -1,
        -1, -1, 1,  -1, 1,  -1, 1,  1,  1,  -1, 1,  1,  -1, 1,  -1, -1, -1, 1,
        1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, 1,  1,  1,  -1, 1,  1,  -1, 1,
        1,  -1, -1, -1, 1,  -1, 1,  1,  -1, 1,  1,  -1, -1, 1,  1,  -1, 1,  -1,
        -1, 1,  -1, 1,  1,  -1, 1,  1,  1,  1,  -1, -1, -1, -1, -1, 1,  1,  1,
        1,  1,  1,  1,  1,  1,  -1, -1, 1,  1,  1,  1,  -1, -1, -1, 1,  1,  -1,
        1,  1,  1,  1};

    // Esegui la ricostruzione del pattern
    // corruptedPattern = hn.resolvePattern(corruptedPattern);
    // corruptedPattern = hn.resolvePattern(corruptedPattern);
    // corruptedPattern = hn.resolvePattern(corruptedPattern);
    // corruptedPattern = hn.resolvePattern(corruptedPattern);
    std::vector<int> resolvedPattern = hn.resolvePattern(corruptedPattern);

    // La soluzione attesa è il pattern originale
    std::vector<int> expectedSolution = {
        1,  -1, 1,  -1, 1,  -1, 1,  -1, -1, -1, 1,  -1, -1, 1,  -1, 1,  1,  -1,
        1,  -1, 1,  1,  -1, -1, -1, 1,  -1, -1, -1, 1,  -1, 1,  1,  -1, 1,  -1,
        1,  1,  -1, 1,  1,  1,  1,  -1, -1, 1,  1,  1,  -1, -1, 1,  1,  1,  -1,
        -1, 1,  1,  -1, 1,  1,  1,  -1, 1,  1,  -1, -1, -1, 1,  -1, 1,  -1, -1,
        -1, -1, 1,  -1, -1, 1,  1,  -1, 1,  -1, 1,  -1, 1,  1,  1,  1,  -1, 1,
        1,  1,  -1, 1,  -1, 1,  1,  -1, 1,  1,  1,  -1, -1, 1,  1,  -1, 1,  -1,
        -1, -1, 1,  -1, 1,  -1, -1, -1, -1, 1,  1,  -1, -1, 1,  -1, 1,  1,  1,
        -1, -1, -1, 1,  -1, -1, 1,  1,  1,  -1, 1,  -1, 1,  1,  -1, -1, -1, -1,
        -1, 1,  -1, 1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, -1, 1,  -1, 1,  -1,
        -1, -1, 1,  -1, 1,  -1, -1, 1,  1,  -1, 1,  1,  -1, 1,  -1, -1, -1, 1,
        1,  1,  -1, 1,  -1, -1, -1, -1, -1, -1, 1,  1,  1,  -1, 1,  1,  -1, 1,
        1,  -1, -1, -1, 1,  -1, 1,  1,  -1, 1,  -1, -1, -1, 1,  1,  -1, 1,  -1,
        -1, 1,  -1, 1,  1,  -1, 1,  1,  1,  1,  -1, -1, -1, -1, -1, 1,  1,  1,
        1,  1,  1,  1,  1,  1,  -1, -1, 1,  1,  1,  1,  -1, -1, -1, -1, 1,  -1,
        1,  1,  1,  1};

    // Verifica che il pattern risolto corrisponda all'originale
    CHECK(resolvedPattern.size() == expectedSolution.size());
    for (size_t i = 0; i < expectedSolution.size(); ++i) {
      CAPTURE(i);
      CHECK(resolvedPattern[i] == expectedSolution[i]);
    }
  }

  SUBCASE("quinto test") {}
}