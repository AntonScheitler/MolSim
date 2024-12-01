#include "../../../../src/particle/container/ParticleContainerDirectSum.h"
#include "../../../../src/computations/positions/PositionComputations.h"
#include <gtest/gtest.h>

/**
 * @brief Tests the position computations (src/computations/positions) via analytically calculated examples
 */
class PositionComputationsTest : public testing::Test {
protected:

    ParticleContainerDirectSum particles;

    // executed before each test
    void SetUp() override {

        Particle a{0};
        Particle b{0};

        // init positions
        a.setX({{3, 5, 4}});
        b.setX({{7, 3, 0}});

        // init mass
        a.setM(6);
        b.setM(1);

        // init velocity
        a.setV({{7, 1, 9}});
        b.setV({{5, 10, 5}});

        // init old force for position calc
        a.setF({{9, 0, 7}});
        b.setF({{2, 10, 6}});

        particles = ParticleContainerDirectSum({{a,b}});
    }
};


/**
 * @brief checks if the stoermer verlet position calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(PositionComputationsTest, StoermerVerletPositionCalcTest) {

    PositionComputations::stoermerVerlet(particles, 0.1);
    // particle a
    EXPECT_NEAR(particles.getParticle(0).getX()[0], 3.7075, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getX()[1], 5.1, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getX()[2], 4.905833333, 0.00000001);
    // particle b
    EXPECT_NEAR(particles.getParticle(1).getX()[0], 7.51, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getX()[1], 4.05, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getX()[2], 0.53, 0.00000001);
}

