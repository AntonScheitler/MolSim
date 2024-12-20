#include "../src/particle/container/ParticleContainerDirectSum.h"
#include <gtest/gtest.h>

class ParticleContainerDirectSumTest : public testing::Test {
    protected:
        ParticleContainerDirectSum empty;
        ParticleContainerDirectSum container;

        void SetUp() override {
            empty = ParticleContainerDirectSum();
            container = ParticleContainerDirectSum();
            for (double d = 0; d < 4; d++) {
                container.addParticle(Particle({d, d, d}, {d, d, d}, d));
            }
        }
};

/**
 * @brief checks if there is nothing returned when iterating through an empty
 * ParticleContainerDirectSum
 */
TEST_F(ParticleContainerDirectSumTest, EmptyParticleContainerDirectSumIteratorTest) {
    for (auto it = empty.begin(); *it != *(empty.end()); ++*it) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if the Particles returned by the Iterator are in the correct
 * order
 */
TEST_F(ParticleContainerDirectSumTest, ParticleContainerDirectSumIteratorTest) {
    double d = 0; // dummy value for x, v and m
    for (auto it = empty.begin(); *it != *(empty.end()); ++*it) {
        Particle particle = **it;
        Particle otherParticle = Particle({d, d, d}, {d, d, d}, d);
        EXPECT_TRUE(particle == otherParticle);
        d++;
    }
}

/**
 * @brief checks if there is nothing returned when iterating through the pairs
 * of an empty ParticleContainerDirectSum
 */
TEST_F(ParticleContainerDirectSumTest, EmptyParticleContainerDirectSumPairIteratorTest) {
    for (auto it = empty.beginPairParticle(); *it != *(empty.endPairParticle()); ++*it) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if the Particle pairs returned by the PairParticleIterator are correct and the number of pairs
 * is correct
 */
TEST_F(ParticleContainerDirectSumTest, ParticleContainerDirectSumPairIteratorTest) {
    std::vector<std::pair<Particle, Particle>> otherPairs = {};
    for (double i = 0; i < 3; i++) {
        for (double j = i + 1; j < 4; j++) {
            otherPairs.push_back(std::pair(Particle({i, i, i}, {i, i, i}, i),
                        Particle({j, j, j}, {j, j, j}, j)));
        }
    }
    size_t pairIndex = 0;
    for (auto it = container.beginPairParticle(); *it != *(container.endPairParticle()); ++*it, pairIndex++) {
        std::pair<Particle, Particle> pair = **it;
        EXPECT_TRUE(pair.first == otherPairs[pairIndex].first && pair.second == otherPairs[pairIndex].second);
    }
    // check that the number of pairs is correct
    EXPECT_TRUE(pairIndex == 6);
}
