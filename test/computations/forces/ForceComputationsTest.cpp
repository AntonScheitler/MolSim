#include "../../../../src/computations/forces/ForceComputations.h"
#include <gtest/gtest.h>
#include "../../../src/spdlogConfig.h"
#include "../../../src/particle/container/ParticleContainerDirectSum.h"

/**
 * @brief Tests the force computations (src/computations/forces) via analytically calculated examples
 */
class ForceComputationsTest : public testing::Test {
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

        particles = ParticleContainerDirectSum({std::vector<Particle>{a, b}});
        SPDLOG_INFO("ForceComputationsTest setUp done");
    }
};


/**
 * @brief checks if the lennard jones force calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(ForceComputationsTest, LennardJonesForceCalcTest) {
    ForceComputations::computeLennardJonesPotential(particles, 5, 1);

    // the analytical solution F_ij calculates the force for j (for i is inverted)
    // particle a
    EXPECT_NEAR(particles.getParticle(0).getF()[0], 0.000285767, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[1], -0.000142883, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[2], -0.000285767, 0.00000001);
    // particle b
    EXPECT_NEAR(particles.getParticle(1).getF()[0], -0.000285767, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[1], 0.000142883, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[2], 0.000285767, 0.00000001);
}

/**
 * @brief checks if the stoermer verlet gravity force calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(ForceComputationsTest, GravityCalcTest) {

    ForceComputations::computeGravity(particles);

    // the analytical solution F_ij calculates the force for j (for i is inverted)
    // particle a
    EXPECT_NEAR(particles.getParticle(0).getF()[0], 0.111111111, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[1], -0.055555555, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[2], -0.111111111, 0.00000001);
    // particle b
    EXPECT_NEAR(particles.getParticle(1).getF()[0], -0.111111111, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[1], 0.055555555, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[2], 0.111111111, 0.00000001);
}


