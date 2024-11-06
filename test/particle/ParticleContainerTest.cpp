#include "../../src/particle/ParticleContainer.h"
#include <gtest/gtest.h>

class ParticleContainerTest : public testing::Test {
    protected:
        ParticleContainer empty;
        ParticleContainer container;

        void SetUp() override {
            empty = ParticleContainer();
            container = ParticleContainer();
            for (double d = 0; d < 4; d++) {
                container.addParticle(Particle({d, d, d}, {d, d, d}, d));
            }
        }
};

/**
 * @brief checks if there is nothing returned when iterating through an empty ParticleContainer
 */
TEST_F(ParticleContainerTest, EmptyParticleContainerIteratorTest) {
    for (Particle& _ : empty) {
        ASSERT_TRUE(false);
    }
}

/**
 * @brief checks if the Particles are returned by the Iterator in the correct order
 */
TEST_F(ParticleContainerTest, ParticleContainerIteratorTest) {
    double d = 0;
    for (Particle particle : container) {
        Particle otherParticle = Particle({d, d, d}, {d, d, d}, d);
        EXPECT_EQ(particle.getM(), otherParticle.getM());
        d++;
    }
}

/**
 * @brief checks if there is nothing returned when iterating through the pairs of an empty ParticleContainer
 */
TEST_F(ParticleContainerTest, EmptyParticleContainerPairIteratorTest) {
    for (auto pair = empty.beginPairParticle(); pair != empty.endPairParticle(); ++pair) {
        ASSERT_TRUE(false);
    }
}
