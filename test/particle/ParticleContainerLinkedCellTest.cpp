#include <particle/container/ParticleContainerLinkedCell.h>
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
                return 0;
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
        ParticleContainerLinkedCell pairsContainer{domainSize2, cutoffRadius, {{ghost, ghost}, {ghost, ghost}, {outflow, outflow}}};

        void SetUp() override {

            // container: 3x3 mesh of cells, 2 particles per cell
            for (double y = 0; y < 3; y++) {
                for (double x = 0; x < 3; x++) {
                    std::array<double, 3> pos1 = {x * container.getCellSize()[0] + 7, y * container.getCellSize()[1] + 7, 0.5 };
                    std::array<double, 3> pos2 = {x * container.getCellSize()[0] + 10, y * container.getCellSize()[1] + 10, 0.5 };
                    Particle p1 = {pos1, {0, 0, 0}, 1};
                    Particle p2 = {pos2, {0, 0, 0}, 1};
                    container.addParticle(p1);
                    container.addParticle(p2);
                    // add all particles to the set
                    particlesSet.insert(p1);
                    particlesSet.insert(p2);
                }
            }
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
    // pairsContainer: single row of cells, 2 particles per cell
    for (double x = 0; x < 3; x++) {
        std::array<double, 3> pos1 = {x * pairsContainer.getCellSize()[0] + 7, 7, 0.5 };
        std::array<double, 3> pos2 = {x * pairsContainer.getCellSize()[0] + 10, 7, 0.5 };
        Particle p1 = {pos1, {0, 0, 0}, 1};
        Particle p2 = {pos2, {0, 0, 0}, 1};
        pairsContainer.addParticle(p1);
        pairsContainer.addParticle(p2);
        particlesVector.push_back(p1);
        particlesVector.push_back(p2);
    }
    // add all pairs to the set
    for (size_t start = 0; start < 3; start += 2) {
        for (size_t i = start; i < start + 4 - 1; i++) {
            for (size_t j = i + 1; j < start + 4; j++) {
                pairsSet.insert(std::make_pair(particlesVector[i], particlesVector[j]));
            }
        }
    }
    // count the number pairs produced when iterating
    size_t count = 0;
    // test if all pairs are returned
    for (auto it = pairsContainer.beginPairParticle(); *it != *(pairsContainer.endPairParticle()); ++*it) {
        std::pair<Particle&, Particle&> pair = **it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(pairsSet.size() == count);


    // clear the middle cell
    pairsContainer.getMesh()[1].getParticles().clear();
    // update the expected pairs
    pairsSet.clear();
    pairsSet.insert(std::make_pair(particlesVector[0], particlesVector[1]));
    pairsSet.insert(std::make_pair(particlesVector[4], particlesVector[5]));
    // reset count
    count = 0;
    // test if all pairs are returned
    for (auto it = pairsContainer.beginPairParticle(); *it != *(pairsContainer.endPairParticle()); ++*it) {
        std::pair<Particle&, Particle&> pair = **it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(pairsSet.size() == count);
}


/**
 * @brief checks the ghost particles for all particles in the boundary are computed correctly
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellGhostParticleTest) {

}
