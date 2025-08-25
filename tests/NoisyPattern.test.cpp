
#include "../HopfieldSimulator/NoisyPattern.hpp"
#include "../HopfieldSimulator/math/RandomUtils.hpp"
#include "doctest.h"

TEST_SUITE("NoisyPattern") {

  TEST_CASE("NoisyPattern") {
    auto points = MD::getMathematicalVertex<int>();
    std::vector<int> source_pattern = {
        -1, -1, 1,  -1, -1, -1, -1, -1, -1, 1,  -1, -1, 1, 1,
        -1, 1,  1,  1,  1,  -1, 1,  1,  1,  1,  1,  1,  1, 1,
        -1, -1, -1, -1, -1, -1, 1,  1,  1,  -1, 1,  -1, 1};

    NP::NoisyPattern<int> np_no_noise(source_pattern, 0.0f);
    CHECK(np_no_noise.getPattern() == source_pattern);

    RU::seedGenerator(1);
    NP::NoisyPattern<int> np_full_noise(source_pattern, 1.0f);
    REQUIRE(np_full_noise.getPattern().size() == source_pattern.size());
    CHECK(np_full_noise.getPattern() != source_pattern);

    CHECK_THROWS_AS(NP::NoisyPattern<int>(source_pattern, -0.1f),
                    std::invalid_argument);
    CHECK_THROWS_AS(NP::NoisyPattern<int>(source_pattern, 1.1f),
                    std::invalid_argument);

    NP::NoisyPattern<int> np_cycle(source_pattern, 0.0f);
    REQUIRE(np_cycle.getPattern() == source_pattern);

    np_cycle.cyclePixelState(0);
    CHECK(np_cycle.getPattern()[0] == points[(0 + 1) % points.size()]);
    CHECK(np_cycle.getPattern()[1] == source_pattern[1]);

    np_cycle.cyclePixelState(0);
    CHECK(np_cycle.getPattern()[0] == points[(0 + 2) % points.size()]);

    CHECK_THROWS_AS(np_cycle.cyclePixelState(source_pattern.size()),
                    std::invalid_argument);
  }
}
