#include "../HopfieldSimulator/CoherenceSetPattern.hpp"
#include "../HopfieldSimulator/math/RandomUtils.hpp"
#include "doctest.h"

TEST_SUITE("CoherenceSetPattern") {

  TEST_CASE("CPS") {

    const std::string simple_image_path_csp = "tests/image_test.png";
    auto points = MD::getMathematicalVertex<int>();

    CSP::CoherenceSetPattern<int> csp_img(simple_image_path_csp, 10, 20);
    CHECK(csp_img.getTrainingPatternVector()[0] == points[0]);
    CHECK(csp_img.getTrainingPatternVector()[1] == points[0]);
    CHECK(csp_img.getRow() == 20);
    CHECK(csp_img.getCol() == 10);

    CHECK(csp_img.getNoisyPatternVector() !=
          csp_img.getTrainingPatternVector());
    CHECK(csp_img.getEvolvingPatternVector() ==
          csp_img.getNoisyPatternVector());

    RU::seedGenerator(200);
    CSP::CoherenceSetPattern<int> csp_rand(0.0f, 2, 2);
    REQUIRE(csp_rand.getTrainingPatternVector().size() == 4);
    CHECK(csp_rand.getNoisyPatternVector() ==
          csp_rand.getTrainingPatternVector());
    CHECK(csp_rand.getEvolvingPatternVector() ==
          csp_rand.getNoisyPatternVector());

    CHECK(csp_rand.getRow() == 2);
    CHECK(csp_rand.getCol() == 2);

    CSP::CoherenceSetPattern<int> csp_same_dim(0.0f, 2, 2);
    CSP::CoherenceSetPattern<int> csp_diff_dim(0.0f, 1, 4);
    CHECK(csp_rand.hasSameDimensionOf(csp_same_dim));
    CHECK_FALSE(csp_rand.hasSameDimensionOf(csp_diff_dim));

    RU::seedGenerator(201);
    std::vector<int> initial_noisy = csp_rand.getNoisyPatternVector();
    csp_rand.flipNoisyPixel(0);

    CHECK(csp_rand.getNoisyPatternVector()[0] ==
          points[(0 + 1) % points.size()]);
    CHECK(csp_rand.getEvolvingPatternVector()[0] ==
          points[(0 + 1) % points.size()]);
    CHECK(csp_rand.getNoisyPatternVector()[1] == initial_noisy[1]);

    csp_rand.reCorrupt(0.0f);
    CHECK(csp_rand.getNoisyPatternVector() ==
          csp_rand.getTrainingPatternVector());
    CHECK(csp_rand.getEvolvingPatternVector() ==
          csp_rand.getTrainingPatternVector());

    csp_rand.regrid(1, 1);
    CHECK(csp_rand.getTrainingPatternVector().size() == 1);
    CHECK(csp_rand.getNoisyPatternVector().size() == 1);
    CHECK(csp_rand.getEvolvingPatternVector().size() == 1);
    CHECK(csp_rand.getRow() == 1);
    CHECK(csp_rand.getCol() == 1);
    CHECK(csp_rand.getNoisyPatternVector() ==
          csp_rand.getTrainingPatternVector());
    CHECK(csp_rand.getEvolvingPatternVector() ==
          csp_rand.getTrainingPatternVector());

    CHECK_THROWS_AS(csp_rand.updateEvolvingState({points[0], points[1]}),
                    std::invalid_argument);

    csp_rand.clearEnergy();
    CHECK(csp_rand.getEnergy().empty());
  }
}
