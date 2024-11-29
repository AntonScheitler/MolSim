#include "../src/particle/container/ParticleContainerLinkedCell.h"
#include <gtest/gtest.h>

class ParticleContainerLinkedCellTest : public testing::Test {
    protected:
        ParticleContainerLinkedCell empty;
        ParticleContainerLinkedCell container;

        void SetUp() override {
        }
};

/**
 * @brief checks if there is nothing returned when iterating through an empty
 * ParticleContainerLinkedCell
 */
TEST_F(ParticleContainerLinkedCellTest, EmptyParticleContainerLinkedCellIteratorTest) {
    for (Particle &_ : empty) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if the Particles returned by the Iterator are in the correct
 * order
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellIteratorTest) {
    double d = 0; // dummy value for x, v and m
    for (Particle particle : container) {
        Particle otherParticle = Particle({d, d, d}, {d, d, d}, d);
        EXPECT_TRUE(particle == otherParticle);
        d++;
    }
}

/**
 * @brief checks if there is nothing returned when iterating through the pairs
 * of an empty ParticleContainerLinkedCell
 */
TEST_F(ParticleContainerLinkedCellTest, EmptyParticleContainerLinkedCellPairIteratorTest) {
    for (auto pair = empty.beginPairParticle(); pair != empty.endPairParticle();
            ++pair) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if the Particle pairs returned by the PairParticleIterator are correct and the number of pairs
 * is correct
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPairIteratorTest) {
}
