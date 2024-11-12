#include "../../../../src/particle/ParticleContainer.h"
#include "../../../../src/computations/forces/ForceComputations.cpp"
#include <gtest/gtest.h>

class ForceComputationsTest : public testing::Test {
protected:

    // variables
    ParticleContainer particles;

    void SetUp() override {

        Particle a{0};
        Particle b{0};

        // init positions
        a.setX({{3, 5, 4}});
        b.setX({{7, 3, 0}});

        particles = ParticleContainer({{a,b}}, 0.1);
    }
};


/**
 * @brief checks if the lennard jones force calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(ForceComputationsTest, LennardJonesForceCalcTest) {
    ForceComputations::computeLennardJonesPotential(particles);

    // the analytical solution F_ij calculates the force for j (for i is inverted)
    ASSERT_NEAR(particles.getParticle(1).getF()[0], 0.000285767, 0.0000001);
    ASSERT_NEAR(particles.getParticle(1).getF()[1], -0.000142883, 0.0000001);
    ASSERT_NEAR(particles.getParticle(1).getF()[2], -0.000285767, 0.0000001);

    ASSERT_NEAR(particles.getParticle(0).getF()[0], -0.000285767, 0.0000001);
    ASSERT_NEAR(particles.getParticle(0).getF()[1], 0.000142883, 0.0000001);
    ASSERT_NEAR(particles.getParticle(0).getF()[2], 0.000285767, 0.0000001);
}
