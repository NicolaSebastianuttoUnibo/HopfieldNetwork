#include "../HopfieldSimulator/EvolvingPattern.hpp"
#include "doctest.h"

TEST_SUITE("EvolvingPattern") {

  TEST_CASE("EvolvingPattern") {

    std::vector<int> initial_pattern = {MD::getMathematicalVertex<int>()[0],
                                        MD::getMathematicalVertex<int>()[1]};

    EP::EvolvingPattern<int> ep(initial_pattern);

    CHECK(ep.getPattern() == initial_pattern);

    ep.getPattern()[0] = MD::getMathematicalVertex<int>()[1];
    CHECK(ep.getPattern()[0] == MD::getMathematicalVertex<int>()[1]);

    CHECK(ep.getEnergy().empty());
    ep.getEnergy().push_back(1.0f);
    CHECK(ep.getEnergy().size() == 1);
    CHECK(ep.getEnergy()[0] == 1.0f);

    ep.clearEnergy();
    CHECK(ep.getEnergy().empty());
  }
}
