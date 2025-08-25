
#include "../HopfieldSimulator/math/RandomUtils.hpp"
#include "doctest.h"

TEST_SUITE("RandomUtils") {

  TEST_CASE("getRandomGenerator returns same instance") {
    std::mt19937 &gen1 = RU::getRandomGenerator();
    std::mt19937 &gen2 = RU::getRandomGenerator();
    CHECK(&gen1 == &gen2);
  }

  TEST_CASE("seedGenerator changes sequence") {
    RU::seedGenerator(123);
    std::mt19937 &gen1 = RU::getRandomGenerator();
    int val1 = gen1();

    RU::seedGenerator(124);
    std::mt19937 &gen2 = RU::getRandomGenerator();
    int val2 = gen2();

    RU::seedGenerator(123);
    std::mt19937 &gen3 = RU::getRandomGenerator();
    int val3 = gen3();

    CHECK(val1 != val2);
    CHECK(val1 == val3);
  }
}
