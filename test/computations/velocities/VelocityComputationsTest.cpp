#include "../../../../src/particle/ParticleContainer.h"
#include "../../../../src/computations/velocities/VelocityComputations.h"
#include <gtest/gtest.h>

class VelocityComputationsTest : public testing::Test {
protected:

    ParticleContainer particles;

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
        a.setOldF({{9, 0, 7}});
        b.setOldF({{2, 10, 6}});

        // new force (for velocity calc)
        a.setF({{1, 6, 9}});
        b.setF({{4, 7, 5}});

        particles = ParticleContainer({{a,b}}, 0.1);
    }
};


/**
 * @brief checks if the störmer verlet velocity calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(VelocityComputationsTest, StoermerVerletVelocityCalcTest) {

    VelocityComputations::stoermerVerlet(particles, 0.1);

    // particle a
    ASSERT_NEAR(particles.getParticle(0).getV()[0], 7.083333333, 0.00000001);
    ASSERT_NEAR(particles.getParticle(0).getV()[1], 1.05, 0.00000001);
    ASSERT_NEAR(particles.getParticle(0).getV()[2], 9.133333333, 0.00000001);
    // particle b
    ASSERT_NEAR(particles.getParticle(1).getV()[0], 5.3, 0.00000001);
    ASSERT_NEAR(particles.getParticle(1).getV()[1], 10.85, 0.00000001);
    ASSERT_NEAR(particles.getParticle(1).getV()[2], 5.55, 0.00000001);
}

