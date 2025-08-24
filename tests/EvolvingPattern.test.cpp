#include "doctest.h"
#include "../HopfieldSimulator/EvolvingPattern.hpp"


TEST_SUITE("EvolvingPattern") {
    
    TEST_CASE("EvolvingPattern") {

        std::vector<int> initial_pattern = {MD::getMathematicalVertex<int>()[0], MD::getMathematicalVertex<int>()[1]};
        
        EP::EvolvingPattern<int> ep(initial_pattern);

        // Constructor & getPattern() const
        CHECK(ep.getPattern() == initial_pattern);
        
        // getPattern() non-const
        ep.getPattern()[0] = MD::getMathematicalVertex<int>()[1];
        CHECK(ep.getPattern()[0] == MD::getMathematicalVertex<int>()[1]);
        
        // getEnergy()
        CHECK(ep.getEnergy().empty());
        ep.getEnergy().push_back(1.0f);
        CHECK(ep.getEnergy().size() == 1);
        CHECK(ep.getEnergy()[0] == 1.0f);

        // clearEnergy()
        ep.clearEnergy();
        CHECK(ep.getEnergy().empty());
    }
}
