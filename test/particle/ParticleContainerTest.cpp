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
 * @brief checks if there is nothing returned when iterating through an empty
 * ParticleContainer
 */
TEST_F(ParticleContainerTest, EmptyParticleContainerIteratorTest) {
    for (Particle &_ : empty) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if the Particles are returned by the Iterator in the correct
 * order
 */
TEST_F(ParticleContainerTest, ParticleContainerIteratorTest) {
    double d = 0;
    for (Particle particle : container) {
        Particle otherParticle = Particle({d, d, d}, {d, d, d}, d);
        EXPECT_TRUE(particle == otherParticle);
        d++;
    }
}

/**
 * @brief checks if there is nothing returned when iterating through the pairs
 * of an empty ParticleContainer
 */
TEST_F(ParticleContainerTest, EmptyParticleContainerPairIteratorTest) {
    for (auto pair = empty.beginPairParticle(); pair != empty.endPairParticle();
            ++pair) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if the Particles are returned by the Iterator in the correct
 * order
 */
TEST_F(ParticleContainerTest, ParticleContainerPairIteratorTest) {
    size_t pairIdx = 0;
    std::vector<std::pair<Particle, Particle>> otherPairs = {};
    for (double i = 0; i < 3; i++) {
        for (double j = i + 1; j < 4; j++) {
            otherPairs.push_back(std::pair(Particle({i, i, i}, {i, i, i}, i),
                        Particle({j, j, j}, {j, j, j}, j)));
        }
    }
    for (auto it = container.beginPairParticle();
            it != container.endPairParticle(); ++it) {
        std::pair<Particle, Particle> pair = *it;
        EXPECT_TRUE(pair.first == otherPairs[pairIdx].first && pair.second == otherPairs[pairIdx].second);
        pairIdx++;
    }
    EXPECT_TRUE(pairIdx == 6);
}
