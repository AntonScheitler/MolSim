#include <particle/container/ParticleContainerLinkedCell.h>
#include <cstddef>
#include <gtest/gtest.h>
#include <unordered_set>
#include <utility>
#include <computations/positions/PositionComputations.h>
#include <utils/ArrayUtils.h>
#include "spdlogConfig.h"

class ParticleMembraneTest : public testing::Test {
protected:
    struct PairHash {
        size_t operator()(const std::pair<Particle, Particle> &pair) const {
            return 0;
        }
    };

    struct PairEqual {
        bool operator()(const std::pair<Particle, Particle> &p1, const std::pair<Particle, Particle> &p2) const {
            return (p1.first.getX() == p2.first.getX() && p1.second.getX() == p2.second.getX()) || (
                       p1.first.getX() == p2.second.getX() && p1.second.getX() == p2.first.getX());
        }
    };

    void SetUp() override {
    }
};

/**
 * @brief checks if the neighboring particles of a membrane are returned correctly by the iterator
 */
TEST_F(ParticleMembraneTest, ParticleMembraneNeighborIterationTest) {
    ParticleContainerLinkedCell container{
        {99, 99, 1}, 33,
        {{periodic, periodic}, {outflow, outflow}, {outflow, outflow}}
    };
    std::unordered_set<std::pair<Particle, Particle>, PairHash, PairEqual> pairsSet{};
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            // create particles
            Particle newParticle = {{x * 33.0 + 11, y * 33.0 + 11, 0}, {0, 0, 0}, 1};
            // add neighbors
            int bottomNeighbor = y == 0 ? -1 : (y - 1) * 3 + x;
            int leftNeighbor = x == 0 ? -1 : y * 3 + (x - 1);
            int rightNeighbor = x == 2 ? -1 : y * 3 + (x + 1);
            int topNeighbor = y == 2 ? -1 : (y + 1) * 3 + x;
            newParticle.addNeighborIdx(bottomNeighbor);
            newParticle.addNeighborIdx(leftNeighbor);
            newParticle.addNeighborIdx(rightNeighbor);
            newParticle.addNeighborIdx(topNeighbor);

            int bottomLeftNeighbor = x == 0 || y == 0 ? -1 : (y - 1) * 3 + (x - 1);
            int bottomRightNeighbor = x == 2 || y == 0 ? -1 : (y - 1) * 3 + (x + 1);
            int topLeftNeighbor = x == 0 || y == 2 ? -1 : (y + 1) * 3 + (x - 1);
            int topRightNeighbor = x == 2 || y == 2 ? -1 : (y + 1) * 3 + (x + 1);
            newParticle.addDiagNeighborIdx(bottomLeftNeighbor);
            newParticle.addDiagNeighborIdx(bottomRightNeighbor);
            newParticle.addDiagNeighborIdx(topLeftNeighbor);
            newParticle.addDiagNeighborIdx(topRightNeighbor);

            // add particle to the container
            container.addParticle(newParticle);
        }
    }

    // insert all expected pairs
    for (Particle &particle: container) {
        for (int directIdx: particle.getAllDirectNeighborIndices()) {
            if (directIdx != -1) {
                pairsSet.insert(std::make_pair(particle, container.getParticle(directIdx)));
            }
        }

        for (int diagIdx: particle.getAllDiagonalNeighborIndices()) {
            if (diagIdx != -1) {
                pairsSet.insert(std::make_pair(particle, container.getParticle(diagIdx)));
            }
        }
    }

    size_t count = 0;
    // test if all direct neighbors are returned
    for (auto it = container.beginMembraneDirectNeighbor(); it != (container.endMembraneDirectNeighbor()); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    // test if all diagonal neighbors are returned
    for (auto it = container.beginMembraneDiagonalNeighbor(); it != (container.endMembraneDiagonalNeighbor()); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(pairsSet.size() == count);
}
