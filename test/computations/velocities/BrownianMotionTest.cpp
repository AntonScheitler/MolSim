#include "../../../../src/particle/container/ParticleContainerDirectSum.h"
#include <gtest/gtest.h>
#include "../../../../src/computations/velocities/VelocityComputations.h"

/**
 * @brief Tests the brownian motion that is applied on particles
 */
class BrownianMotionTest : public testing::Test {
protected:
    void SetUp() override {
    };
};

/**
 * @brief test if the brownian motion velocity applied in 2D is in the correct range for x and y coords,
 * and the z-velocity is not changed
 */
TEST_F(BrownianMotionTest, BrownianMotion2DTest) {
    double averageVelocity = 0.1;

    Particle particle{{0, 0, 0}, {1.0, 1.0, 10.0}, 1, 0};
    std::vector<Particle> particles{particle};
    ParticleContainerDirectSum container{particles};

    VelocityComputations::applyBrownianMotion(container, averageVelocity, 1, 2);

    Particle &p = container.getParticle(0);

    EXPECT_TRUE(p.getV()[0] >= 1 - 5 * averageVelocity && p.getV()[0] <= 1 + 5 * averageVelocity);
    EXPECT_TRUE(p.getV()[1] >= 1 - 5 * averageVelocity && p.getV()[1] <= 1 + 5 * averageVelocity);
    EXPECT_FLOAT_EQ(p.getV()[2], 10.0);
}

/**
 * @brief checks if the brownian motion velocity applied in 3D is in the correct range for x, y and z coords
 */
TEST_F(BrownianMotionTest, BrownianMotion3DTest) {
    double averageVelocity = 0.1;

    Particle particle{{0, 0, 0}, {1.0, 1.0, 1.0}, 1, 0};
    std::vector<Particle> particles{particle};
    ParticleContainerDirectSum container{particles};

    VelocityComputations::applyBrownianMotion(container, averageVelocity, 1, 3);

    Particle &p = container.getParticle(0);

    EXPECT_TRUE(p.getV()[0] >= 1 - 5 * averageVelocity && p.getV()[0] <= 1 + 5 * averageVelocity);
    EXPECT_TRUE(p.getV()[1] >= 1 - 5 * averageVelocity && p.getV()[1] <= 1 + 5 * averageVelocity);
    EXPECT_TRUE(p.getV()[2] >= 1 - 5 * averageVelocity && p.getV()[2] <= 1 + 5 * averageVelocity);
}
