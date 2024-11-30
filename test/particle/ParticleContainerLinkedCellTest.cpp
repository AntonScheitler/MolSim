#include "../src/particle/container/ParticleContainerLinkedCell.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <unordered_set>
#include <utility>


class ParticleContainerLinkedCellTest : public testing::Test {
    protected:
        struct ParticleHash {
            size_t operator()(const Particle& p) const {
                return 0;
            }
        };
        struct ParticleEqual {
            bool operator()(const Particle& p1, const Particle& p2) const {
                return p1 == p2;
            }
        };

        struct PairHash {
            size_t operator()(const std::pair<Particle, Particle>& pair) const {
                return (std::hash<double>()(pair.first.getX()[0]) << 6) ^
                    (std::hash<double>()(pair.first.getX()[1]) << 4) ^
                    (std::hash<double>()(pair.first.getX()[2]) << 2) ^
                    (std::hash<double>()(pair.second.getX()[0]) << 6) ^
                    (std::hash<double>()(pair.second.getX()[1]) << 4) ^
                    (std::hash<double>()(pair.second.getX()[2]) << 2);
            }
        };
        struct PairEqual {
            bool operator()(const std::pair<Particle, Particle>& p1, const std::pair<Particle, Particle>& p2) const {
                return (p1.first == p2.first && p1.second == p2.second) || (p1.first == p2.second && p1.second == p2.first);
            }
        };
        std::unordered_set<std::pair<Particle, Particle>, PairHash, PairEqual> pairsSet;
        std::unordered_set<Particle, ParticleHash, ParticleEqual> particlesSet;
        std::vector<Particle> particlesVector;

        std::array<double, 3> domainSize1 = {99, 99, 1};
        std::array<double, 3> domainSize2 = {99, 33, 1};
        double cutoffRadius = 33;
        ParticleContainerLinkedCell empty{domainSize1, cutoffRadius};
        ParticleContainerLinkedCell container{domainSize1, cutoffRadius};
        ParticleContainerLinkedCell pairsContainer{domainSize2, cutoffRadius};

        void SetUp() override {
            for (double y = 0; y < 3; y++) {
                std::array<double, 3> pos = {y * pairsContainer.getCellSize()[0] + 7, 7, 0.5 };
                Particle p = {pos, {0, 0, 0}, 1};
                pairsContainer.addParticle(p);

                std::array<double, 3> pos3 = {y * pairsContainer.getCellSize()[0] + 10, 7, 0.5 };
                Particle p3 = {pos3, {0, 0, 0}, 1};
                pairsContainer.addParticle(p3);

                for (double x = 0; x < 3; x++) {
                    // create two particles per cell
                    std::array<double, 3> pos1 = {x * container.getCellSize()[0] + 7, y * container.getCellSize()[1] + 7, 0.5 };
                    Particle p1 = {pos1, {0, 0, 0}, 1};
                    container.addParticle(p1);

                    std::array<double, 3> pos2 = {x * container.getCellSize()[0] + 10, y * container.getCellSize()[1] + 10, 0.5 };
                    Particle p2 = {pos2, {0, 0, 0}, 1};
                    container.addParticle(p2);

                    // add the particles to the vector 
                    particlesVector.push_back(p1);
                    particlesVector.push_back(p2);
                    particlesSet.insert(p1);
                    particlesSet.insert(p2);
                }
            }
            pairsSet.insert(std::make_pair(particlesVector[0], particlesVector[1]));
            pairsSet.insert(std::make_pair(particlesVector[0], particlesVector[2]));
            pairsSet.insert(std::make_pair(particlesVector[0], particlesVector[3]));
            pairsSet.insert(std::make_pair(particlesVector[1], particlesVector[2]));
            pairsSet.insert(std::make_pair(particlesVector[1], particlesVector[3]));
            pairsSet.insert(std::make_pair(particlesVector[2], particlesVector[3]));
            pairsSet.insert(std::make_pair(particlesVector[2], particlesVector[4]));
            pairsSet.insert(std::make_pair(particlesVector[2], particlesVector[5]));
            pairsSet.insert(std::make_pair(particlesVector[3], particlesVector[4]));
            pairsSet.insert(std::make_pair(particlesVector[3], particlesVector[5]));
            pairsSet.insert(std::make_pair(particlesVector[4], particlesVector[5]));
        }
};

/**
 * @brief checks if there is nothing returned when iterating through an empty ParticleContainerLinkedCell
 */
TEST_F(ParticleContainerLinkedCellTest, EmptyParticleContainerLinkedCellIteratorTest) {
    for (auto it = empty.begin(); *it != *(empty.end()); ++*it) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if all particles are returned by the iterator
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellIteratorTest) {
    size_t count = 0;
    for (auto it = container.begin(); *it != *(container.end()); ++*it) {
        Particle particle = **it;
        EXPECT_TRUE(particlesSet.count(particle) == 1);
        particlesSet.erase(particle);
        count++;
    }
    EXPECT_TRUE(count == 18);
    EXPECT_TRUE(particlesSet.size() == 0);
}

/**
 * @brief checks if there is nothing returned when iterating through the pairs of an empty ParticleContainerLinkedCell
 */
TEST_F(ParticleContainerLinkedCellTest, EmptyParticleContainerLinkedCellPairIteratorTest) {
    for (auto it = empty.beginPairParticle(); *it != *(empty.endPairParticle()); ++*it) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if all particle pairs are returned by the iterator 
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPairIteratorTest) {
    size_t count = 0;
    for (auto it = pairsContainer.beginPairParticle(); *it != *(pairsContainer.endPairParticle()); ++*it) {
        std::pair<Particle&, Particle&> pair = **it;
        //EXPECT_TRUE(pairsSet.count(pair) == 1);
        //pairsSet.erase(pair);
        count++;
    }
    EXPECT_TRUE(count == 11);
    //EXPECT_TRUE(pairsSet.size() == 0);
}