
#include "doctest.h"
#include "../HopfieldSimulator/ITrainingPattern.hpp"
#include "../HopfieldSimulator/math/RandomUtils.hpp"






TEST_SUITE("TrainingPattern") {
    
    TEST_CASE("TrainingImagePattern") {
    
        const std::string simple_image_path = "tests/image_test.png";
        auto points = MD::getMathematicalVertex<int>();
        size_t points_size = points.size();
        const float maxLum = 256.0f; 

        // Original image (3x2):
        // 0   128 255
        // 255 128 0

        // Test with 3x2 image, regrid to 3x2 (same size)
        std::vector<int> expected_3x2_pattern;
        std::vector<float> avg_lums_3x2 = {0.0f, 128.0f, 255.0f, 255.0f, 128.0f, 0.0f}; // Directly from image
        for (float lum : avg_lums_3x2) {
            size_t index = static_cast<size_t>(lum / maxLum * points_size);
            if (index >= points_size) index = points_size - 1;
            expected_3x2_pattern.push_back(points[index]);
        }
        TP::ImageTrainingPattern<int> imgPattern(simple_image_path, 3, 2);
        REQUIRE(imgPattern.getPattern().size() == 6);
        CHECK(imgPattern.getPattern() == expected_3x2_pattern);
        
        // Regrid to 3x1: Average luminances [127.5,180,127.5]
        std::vector<int> expected_3x1_pattern;
        std::vector<float> avg_lums_3x1 = {127.5f,180.0f,127.5f};///calculated manually
        for (float lum : avg_lums_3x1) {
            size_t index = static_cast<size_t>(lum / maxLum * points_size);
            if (index >= points_size) index = points_size - 1;
            expected_3x1_pattern.push_back(points[index]);
        }
        imgPattern.regrid(3, 1);
        REQUIRE(imgPattern.getPattern().size() == 3);
        CHECK(imgPattern.getPattern() == expected_3x1_pattern);

        // Regrid to 1x2: Average luminances [127.66..., 127.66...]
        std::vector<int> expected_1x2_pattern;
        std::vector<float> avg_lums_1x2 = {127.66666f, 127.66666f}; ///calculated manually
        for (float lum : avg_lums_1x2) {
            size_t index = static_cast<size_t>(lum / maxLum * points_size);
            if (index >= points_size) index = points_size - 1;
            expected_1x2_pattern.push_back(points[index]);
        }
        imgPattern.regrid(1, 2);
        REQUIRE(imgPattern.getPattern().size() == 2);
        CHECK(imgPattern.getPattern() == expected_1x2_pattern);

        // Constructor with invalid path
        CHECK_THROWS_AS(TP::ImageTrainingPattern<int>("non_existent_image.png", 1, 1), std::runtime_error);

        // Regrid with zero dimensions
        TP::ImageTrainingPattern<int> imgPattern_for_error(simple_image_path, 1, 1);
        CHECK_THROWS_AS(imgPattern_for_error.regrid(0, 1), std::invalid_argument);
        CHECK_THROWS_AS(imgPattern_for_error.regrid(1, 0), std::invalid_argument);
        CHECK_THROWS_AS(imgPattern_for_error.regrid(0, 0), std::invalid_argument);
    }

    TEST_CASE("RandomImagePattern") {
        
        RU::seedGenerator(100); 
        TP::RandomTrainingPattern<int> randPattern(0.5f, 10, 10);
        REQUIRE(randPattern.getPattern().size() == 100);

        auto points = MD::getMathematicalVertex<int>();
        for (const auto& val : randPattern.getPattern()) {
            bool found = false;
            for (const auto& p : points) {
                if (val == p) {
                    found = true;
                    break;
                }
            }
            CHECK(found);
        }

        RU::seedGenerator(101);
        randPattern.regrid(5, 5);
        REQUIRE(randPattern.getPattern().size() == 25);
        

        RU::seedGenerator(); 
        TP::RandomTrainingPattern<int> blackPattern(0.0f, 17893, 918);
        REQUIRE(blackPattern.getPattern().size() == 17893*918);
 bool found = false;
        for (const auto& val : blackPattern.getPattern()) {
        
                if (val == 1) {
                    found = true;  //this is a problem              
            }
        }
        
            CHECK(!found);


        RU::seedGenerator(10221);
        randPattern.regrid(17, 3);
        REQUIRE(randPattern.getPattern().size() == 51);



        CHECK_THROWS_AS(randPattern.regrid(0, 1), std::invalid_argument);
        CHECK_THROWS_AS(randPattern.regrid(1, 0), std::invalid_argument);
    }


}
