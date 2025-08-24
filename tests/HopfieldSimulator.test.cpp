
#include "doctest.h"

#include "../HopfieldSimulator/HopfieldSimulator.hpp"
#include "../HopfieldSimulator/math/RandomUtils.hpp"

#include <fstream>
#include <filesystem>


TEST_SUITE("HopfieldSimulator") {
    const std::string image_path_hns = "tests/image_test.png";
    const std::string output_training_file = "test.training";


   TEST_CASE("HopfieldSimulator") {
        
        using NeuronsType = int;
        using MatrixType = double;
        auto points = MD::getMathematicalVertex<NeuronsType>();

        HS::HopfieldSimulator<NeuronsType, MatrixType> simulator;

        // emplace_pattern (image)
        simulator.emplace_pattern(image_path_hns, 3, 2); 
        REQUIRE(simulator.size() == 1);
        CHECK(simulator.getPatterns()[0]->getCol() == 3);
        CHECK(simulator.getPatterns()[0]->getRow() == 2);
        
        //emplace_pattern (random)
        RU::seedGenerator(300);
        simulator.emplace_pattern(0.0f, 3, 2); 
        REQUIRE(simulator.size() == 2);
        CHECK(simulator.getPatterns()[1]->getCol() == 3);
        CHECK(simulator.getPatterns()[1]->getRow() == 2);
        
        // // generatePattern (alias for emplace_pattern(random))
        simulator.generatePattern(0.1f, 1, 1);
        REQUIRE(simulator.size() == 3);

        // // removePattern
        simulator.removePattern(2); // Remove the last added pattern
        REQUIRE(simulator.size() == 2);
        CHECK_THROWS_AS(simulator.removePattern(100), std::runtime_error); 

        // // regrid
        simulator.regrid(4, 4); // Regrid all existing patterns to 4x4
        CHECK(simulator.getPatterns()[0]->getCol() == 4);
        CHECK(simulator.getPatterns()[0]->getRow() == 4);
        CHECK(simulator.getPatterns()[1]->getCol() == 4);
        CHECK(simulator.getPatterns()[1]->getRow() == 4);

        // // corruptPattern
        simulator.corruptPattern(1, 0.0f); // Make pattern 1 uncorrupted (matches training)
        CHECK(simulator.getPatterns()[1]->getNoisyPatternVector() == simulator.getPatterns()[1]->getTrainingPatternVector());
        
        // // cyclePixelStateOnPattern
        std::vector<NeuronsType> initial_evolving = simulator.getPatterns()[1]->getEvolvingPatternVector();
        simulator.cyclePixelStateOnPattern(1, 0); // Flip first pixel of pattern 1
        CHECK(simulator.getPatterns()[1]->getEvolvingPatternVector()[0] != initial_evolving[0]);

        // // trainNetworkHebb
        std::atomic<float> status_hebb(0.0f);
        simulator.trainNetworkHebb(&status_hebb);
        CHECK(status_hebb.load() > 0.99f);
        CHECK(simulator.checkDimension()); 

        // // resolvePattern
        simulator.resolvePattern(1, nullptr); // Resolve pattern 1 (which was corrupted and flipped)
        CHECK(simulator.getPatterns()[1]->getEvolvingPatternVector() == simulator.getPatterns()[1]->getTrainingPatternVector());
        CHECK_FALSE(simulator.getPatterns()[1]->getEnergy().empty()); 

        // // clear energy
        simulator.clear(1);
        CHECK(simulator.getPatterns()[1]->getEnergy().empty());

        // // saveFileTraining
        simulator.saveFileTraining(output_training_file);
        // Verify file content
        std::ifstream inFile(output_training_file);
        REQUIRE(inFile.is_open());
        size_t numRows, numCols;
        inFile >> numRows >> numCols;
        CHECK(numRows == 4); 
        CHECK(numCols == 4);
        
        std::vector<MatrixType> loaded_matrix_data;
        MatrixType val;
        while (inFile >> val) {
            loaded_matrix_data.push_back(val);
        }
        inFile.close();
        CHECK(simulator.checkDimension());
 

        // // setTraining
        std::vector<MatrixType> new_matrix(64, MatrixType(1.0)); 
        simulator.setTraining(64, 64, new_matrix);
        CHECK(simulator.getPatterns()[0]->getCol() == 64);

        // // trainNetworkWithPseudoinverse
        std::atomic<float> status_pi(0.0f);
        simulator.trainNetworkWithPseudoinverse(&status_pi);
        CHECK(status_pi.load() > 0.99f);
        
      
    }
}
