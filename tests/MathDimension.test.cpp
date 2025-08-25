#include "../HopfieldSimulator/math/MathDimension.hpp"
#include "doctest.h"

TEST_SUITE("MathDimension") {

  TEST_CASE("MathematicalDimension for int") {
    CHECK(MD::getMathematicalDimension<int>() == 1);
    auto points = MD::getMathematicalVertex<int>();
    REQUIRE(points.size() == 2);
    CHECK(points[0] == -1);
    CHECK(points[1] == 1);
    CHECK(MD::getMathematicalNumberVertex<int>() == 2);
  }

  TEST_CASE("MathematicalDimension for std::complex<int>") {
    CHECK(MD::getMathematicalDimension<std::complex<int>>() == 2);
    auto points = MD::getMathematicalVertex<std::complex<int>>();
    REQUIRE(points.size() == 4);
    CHECK(points[0] == std::complex<int>(-1, -1));
    CHECK(points[1] == std::complex<int>(-1, 1));
    CHECK(points[2] == std::complex<int>(1, -1));
    CHECK(points[3] == std::complex<int>(1, 1));
    CHECK(MD::getMathematicalNumberVertex<std::complex<int>>() == 4);
  }

  TEST_CASE("MathematicalDimension for int8_t") {
    CHECK(MD::getMathematicalDimension<int8_t>() == 1);
    auto points = MD::getMathematicalVertex<int8_t>();
    REQUIRE(points.size() == 2);
    CHECK(points[0] == -1);
    CHECK(points[1] == 1);
    CHECK(MD::getMathematicalNumberVertex<int8_t>() == 2);
  }

  TEST_CASE("MathematicalDimension for std::complex<int8_t>") {
    CHECK(MD::getMathematicalDimension<std::complex<int8_t>>() == 2);
    auto points = MD::getMathematicalVertex<std::complex<int8_t>>();
    REQUIRE(points.size() == 4);
    CHECK(points[0] == std::complex<int8_t>(-1, -1));
    CHECK(points[1] == std::complex<int8_t>(-1, 1));
    CHECK(points[2] == std::complex<int8_t>(1, -1));
    CHECK(points[3] == std::complex<int8_t>(1, 1));
    CHECK(MD::getMathematicalNumberVertex<std::complex<int8_t>>() == 4);
  }

  TEST_CASE("MathematicalDimension for std::complex<float>") {
    CHECK(MD::getMathematicalDimension<std::complex<float>>() == 2);
    auto points = MD::getMathematicalVertex<std::complex<float>>();
    REQUIRE(points.size() == 4);
    CHECK(points[0] == std::complex<float>(-1.0f, -1.0f));
    CHECK(points[1] == std::complex<float>(-1.0f, 1.0f));
    CHECK(points[2] == std::complex<float>(1.0f, -1.0f));
    CHECK(points[3] == std::complex<float>(1.0f, 1.0f));
    CHECK(MD::getMathematicalNumberVertex<std::complex<int8_t>>() == 4);
  }
}
