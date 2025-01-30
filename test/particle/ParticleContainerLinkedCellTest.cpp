#include <particle/container/ParticleContainerLinkedCell.h>
#include <cstddef>
#include <gtest/gtest.h>
#include <unordered_set>
#include <utility>
#include <computations/positions/PositionComputations.h>
#include <computations/forces/ForceComputations.h>
#include <computations/velocities/VelocityComputations.h>
#include <utils/ArrayUtils.h>
#include "particle/boundary/Boundary.h"
#include "spdlogConfig.h"

class ParticleContainerLinkedCellTest : public testing::Test {
protected:
    struct ParticleHash {
        size_t operator()(const Particle &p) const {
            return 0;
        }
    };

    struct ParticleEqual {
        bool operator()(const Particle &p1, const Particle &p2) const {
            return p1 == p2;
        }
    };

    struct PairHash {
        size_t operator()(const std::pair<Particle, Particle> &pair) const {
            return 0;
        }
    };

    struct PairEqual {
        bool operator()(const std::pair<Particle, Particle> &p1, const std::pair<Particle, Particle> &p2) const {
            return (p1.first.getX() == p2.first.getX() && p1.second.getX() == p2.second.getX()) || (p1.first.getX() == p2.second.getX() && p1.second.getX() == p2.first.getX());
        }
    };

    ParticleContainerLinkedCell empty{{99, 99, 1}, 33};

    void SetUp() override {
    }
};

/**
 * @brief checks if there is nothing returned when iterating through an empty ParticleContainerLinkedCell
 */
TEST_F(ParticleContainerLinkedCellTest, EmptyParticleContainerLinkedCellIteratorTest) {
    for (Particle& particle : empty) {
        EXPECT_TRUE(false);
    }
}

/**
 * @brief checks if all particles are returned by the iterator
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellIteratorTest) {
    ParticleContainerLinkedCell container{{99, 99, 1}, 33};
    std::unordered_set<Particle, ParticleHash, ParticleEqual> particlesSet{};

    // container: 3x3 mesh of cells, 2 particles per cell
    for (double y = 0; y < 3; y++) {
        for (double x = 0; x < 3; x++) {
            std::array<double, 3> pos1 = {x * container.getCellSize()[0] + 7, y * container.getCellSize()[1] + 7, 0.5};
            std::array<double, 3> pos2 = {x * container.getCellSize()[0] + 10, y * container.getCellSize()[1] + 10,
                                          0.5};
            Particle p1 = {pos1, {0, 0, 0}, 1};
            Particle p2 = {pos2, {0, 0, 0}, 1};
            container.addParticle(p1);
            container.addParticle(p2);
            // add all particles to the set
            particlesSet.insert(p1);
            particlesSet.insert(p2);
        }
    }

    size_t count = 0;
    for (Particle& particle : container) {
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
 * @brief checks if the number of pairs returned by the pair iterator is correct
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPairIteratorNumPairsTest) {
    ParticleContainerLinkedCell pairsContainer{{8, 8, 1}, 2, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};
    for (double y = 0; y < 4; y++) {
        for (double x = 0; x < 4; x++) {
            pairsContainer.addParticle({{2 * x + 1, 2 * y + 1, 0.5}, {0, 0, 0}, 1});
        }
    }

    // count the number pairs produced when iterating
    size_t count = 0;
    // test if all pairs are returned
    for (auto it = pairsContainer.beginPairParticle(); *it != *(pairsContainer.endPairParticle()); ++*it) {
        std::pair<Particle &, Particle &> pair = **it;
        count++;
    }
    EXPECT_TRUE(count == 42);
}

/**
 * @brief checks if all particle pairs are returned by the iterator 
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPairIteratorTest) {
    // contains all pairs that should be returned by the iterator
    std::unordered_set<std::pair<Particle, Particle>, PairHash, PairEqual> pairsSet{};
    // a helper vector to create pairs 
    std::vector<Particle> particlesVector{};
    // single row of cells, 2 particles per cell
    ParticleContainerLinkedCell pairsContainer{{99, 33, 1}, 33, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};
    for (double x = 0; x < 3; x++) {
        std::array<double, 3> pos1 = {x * pairsContainer.getCellSize()[0] + 7, 7, 0.5};
        std::array<double, 3> pos2 = {x * pairsContainer.getCellSize()[0] + 10, 7, 0.5};
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
        std::pair<Particle &, Particle &> pair = **it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(pairsSet.size() == count);


    // clear the middle cell
    pairsContainer.getMesh()[1].getParticlesIndices().clear();
    // update the expected pairs
    pairsSet.clear();
    pairsSet.insert(std::make_pair(particlesVector[0], particlesVector[1]));
    pairsSet.insert(std::make_pair(particlesVector[4], particlesVector[5]));
    // reset count
    count = 0;
    // test if all pairs are returned
    for (auto it = pairsContainer.beginPairParticle(); *it != *(pairsContainer.endPairParticle()); ++*it) {
        std::pair<Particle &, Particle &> pair = **it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(pairsSet.size() == count);
}

TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPairIteratorSingleCellTest) {
    // contains all pairs that should be returned by the iterator
    std::unordered_set<std::pair<Particle, Particle>, PairHash, PairEqual> pairsSet{};
    // single row of cells, 2 particles per cell
    ParticleContainerLinkedCell singleCellContainer{{33, 33, 1}, 33};

    // add particle to container (set remains empty for now)
    Particle p1 = {{7, 7, 0.5}, {0, 0, 0}, 1};
    singleCellContainer.addParticle(p1);
    // test if no pair is returned
    for (auto it = singleCellContainer.beginPairParticle(); *it != *(singleCellContainer.endPairParticle()); ++*it) {
        EXPECT_TRUE(false);
    }

    // add more particles to the cell
    Particle p2 = {{10, 10, 0.5}, {0, 0, 0}, 1};
    Particle p3 = {{13, 13, 0.5}, {0, 0, 0}, 1};
    singleCellContainer.addParticle(p2);
    singleCellContainer.addParticle(p3);
    // add pairs to the set of expected pairs
    pairsSet.insert(std::make_pair(p1, p2));
    pairsSet.insert(std::make_pair(p1, p3));
    pairsSet.insert(std::make_pair(p2, p3));
    // update the expected pairs
    // reset count
    size_t count = 0;
    // test if all pairs are returned
    for (auto it = singleCellContainer.beginPairParticle(); *it != *(singleCellContainer.endPairParticle()); ++*it) {
        std::pair<Particle &, Particle &> pair = **it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(pairsSet.size() == count);
}


/**
 * @brief checks the ghost particles for all particles in the boundary are computed correctly
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellGhostParticleTest) {
    // 3x3 mesh of cells, 1 particle per cell 
    ParticleContainerLinkedCell container{{99, 99, 1}, 33,
                                          {{reflect, reflect}, {reflect, reflect}, {outflow, outflow}}};
    // set of expected ghost/particle pairs
    std::unordered_set<std::pair<Particle, Particle>, PairHash, PairEqual> pairsSet;

    for (double y = 0; y < 3; y++) {
for (double x = 0; x < 3; x++) {
            std::array<double, 3> pos1 = {x * container.getCellSize()[0] + 7, y * container.getCellSize()[1] + 7, 0.5};
            // all particles move to the top-right 
            Particle p1 = {pos1, {1, 1, 0}, 1};
            container.addParticle(p1);
            // add ghost to the left
            if (x == 0) {
                pairsSet.insert(
                        std::make_pair(p1, Particle{{-7, y * container.getCellSize()[1] + 7, 0.5}, {-1, 1, 0}, 1}));
            }
            // add ghost to the right
            if (x == 2) {
                pairsSet.insert(std::make_pair(p1, Particle{
                        {(x + 2) * container.getCellSize()[0] - 7, y * container.getCellSize()[1] + 7, 0.5}, {-1, 1, 0},
                        1}));
            }
            // add ghost to the bottom
            if (y == 0) {
                pairsSet.insert(
                        std::make_pair(p1, Particle{{x * container.getCellSize()[0] + 7, -7, 0.5}, {1, -1, 0}, 1}));
            }
            // add ghost to the top
            if (y == 2) {
                pairsSet.insert(std::make_pair(p1, Particle{
                        {x * container.getCellSize()[0] + 7, (y + 2) * container.getCellSize()[1] - 7, 0.5}, {1, -1, 0},
                        1}));
            }
        }
    }

    // count the number of ghost-particle pairs returned by the iterator 
    size_t count = 0;
    for (auto it = container.beginPairGhost(); it != (container.endPairGhost()); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(count == pairsSet.size());
}

/**
 * @brief checks the ghost particles for all particles in the boundary are computed correctly
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellGhostParticleTestWithSpecialBoundary) {
    // 3x3 mesh of cells, 1 particle per cell 
    ParticleContainerLinkedCell container{{99, 99, 1}, 33,
                                          {{reflect, outflow}, {reflect, outflow}, {outflow, outflow}}};
    // set of expected ghost/particle pairs
    std::unordered_set<std::pair<Particle, Particle>, PairHash, PairEqual> pairsSet;

    for (double y = 0; y < 3; y++) {
        for (double x = 0; x < 3; x++) {
            std::array<double, 3> pos1 = {x * container.getCellSize()[0] + 7, y * container.getCellSize()[1] + 7, 0.5};
            // all particles move to the top-right 
            Particle p1 = {pos1, {1, 1, 0}, 1};
            container.addParticle(p1);
            // add ghost to the left
            if (x == 0) {
                pairsSet.insert(
                        std::make_pair(p1, Particle{{-7, y * container.getCellSize()[1] + 7, 0.5}, {-1, 1, 0}, 1}));
            }
            // add ghost to the bottom
            if (y == 0) {
                pairsSet.insert(
                        std::make_pair(p1, Particle{{x * container.getCellSize()[0] + 7, -7, 0.5}, {1, -1, 0}, 1}));
            }
        }
    }

    // count the number of ghost-particle pairs returned by the iterator 
    size_t count = 0;
    for (auto it = container.beginPairGhost(); it != (container.endPairGhost()); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        EXPECT_TRUE(pairsSet.count(pair) == 1);
        count++;
    }
    EXPECT_TRUE(count == pairsSet.size());
}

/**
 * @brief checks if a particle is inserted in the correct cell and it's position is adjusted correctly when passing
 * through a periodic boundary
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPeriodicBoundaryPositionTest) {
    ParticleContainerLinkedCell container{{99, 33, 1}, 33,
                                          {{periodic, periodic}, {outflow, outflow}, {outflow, outflow}}};
    Particle particle = Particle({98, 10, 0}, {0, 0, 0}, 1);
    container.addParticle(particle);
    // make sure the particle is inserted correctly
    EXPECT_TRUE(container.getMesh()[2].size() == 1);
    EXPECT_TRUE(particle == container.getParticles()[container.getMesh()[2].getParticlesIndices()[0]]);

    container.getParticles()[0].setOldX({98, 10, 0});
    container.getParticles()[0].setX({100, 10, 0});
    container.correctCellMembershipAllParticles();

    // make sure it was moved to the correct cell
    EXPECT_TRUE(container.getMesh()[2].size() == 0);
    EXPECT_TRUE(container.getMesh()[0].size() == 1);
    EXPECT_TRUE(particle == container.getParticles()[container.getMesh()[0].getParticlesIndices()[0]]);

    // make sure the particle's coordinates are correct
    std::array<double, 3> newCoords = {1, 10, 0};
    EXPECT_TRUE(container.getParticles()[0].getX() == newCoords);
}

/**
 * @brief checks if the force between two particles, which are separated by a periodic boundary, is computed correctly
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellPeriodicBoundaryForceTest) {
    double sigma = 1;
    double epsilon = 5;
    ParticleContainerLinkedCell container{{99, 33, 1}, 33,
                                          {{periodic, periodic}, {outflow, outflow}, {outflow, outflow}}};
    container.addParticle({{0.5, 10, 0}, {0, 0, 0}, 1, 0, 5, 1, false});
    container.addParticle({{98.5, 10, 0}, {0, 0, 0}, 1,0, 5, 1, false});
    Particle& particleLeft = container.getParticles()[0];
    Particle& particleRight = container.getParticles()[1];

    ForceComputations::computeLennardJonesPotentialCutoff(container, 33, 1);
    std::array<double, 3> distanceVector = {1, 0, 0};
    double distance = 1;
    // don't consider particles which are further apart than the cutoff radius
    double dist = std::pow(sigma / distance, 2);

    double factor = (-24.0 * epsilon) / std::pow(distance, 2) * (std::pow(dist, 3) - 2 * std::pow(dist, 6));

    std::array<double, 3> forceOnParticleLeft = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
    EXPECT_TRUE(forceOnParticleLeft == particleLeft.getF());
    std::array<double, 3> forceOnParticleRight = ArrayUtils::elementWiseScalarOp(-1, forceOnParticleLeft, std::multiplies<>());
    EXPECT_TRUE(forceOnParticleRight == particleRight.getF());
}


/**
 * @brief checks if particleIndices can be added to and removed from a cell
 */
TEST_F(ParticleContainerLinkedCellTest, CellRemoveParticleTest) {
    // test container cell add and remove particle
    Cell c = Cell{false, 0};
    c.addParticleIdx(0);
    c.removeParticle(0);
    EXPECT_EQ(c.size(), 0);
}

/**
 * @brief checks that the particle flows out of the mesh when all boundaries are outflowing
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellBoundaryOutflowTest) {

    ParticleContainerLinkedCell container{{3, 3, 1}, 1, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};

    /*            outflow
     *           +-------+
     *           |       |
     *   outflow |       | outflow (x)
     *           +-------+
     *            outflow (y)
     */

    // particle a should move out of the high x boundary (outflow) (cell 17)
    Particle a{{1.0, 1.0, 0}, {1.0, 0, 0}, 1, 0, 5, 1, false};

    container.addParticle(a);

    double deltaT = 0.002;
    double epsilon = 5;
    double sigma = 1;

    // run simulation step
    int iterations = 2000;
    for (int i = 0; i < iterations; i++) {
        PositionComputations::updateOldX(container, 1);
        PositionComputations::stoermerVerlet(container, deltaT, 1);
        container.correctCellMembershipAllParticles();

        ForceComputations::resetForces(container, 1);
        ForceComputations::computeLennardJonesPotential(container);
        ForceComputations::computeGhostParticleRepulsion(container, 1);
        VelocityComputations::stoermerVerlet(container, deltaT, 1);
    }

    // check
    bool particlePresent = false;
    for(int i = 0; i < container.getMesh().size(); i++) {
        Cell& c = container.getCell(i);
        auto particles = c.getParticlesIndices();
        if(!particles.empty()) {
            particlePresent = true;
        }
    }
    // expect that particle a (type 0) is not in the container anymore
    EXPECT_FALSE(particlePresent);
}

/**
 * @brief checks that the particles stays in the mesh when all boundaries are reflecting
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellBoundaryReflectTest) {

    ParticleContainerLinkedCell container{{3, 3, 1}, 1, {{reflect, reflect}, {reflect, reflect}, {outflow, outflow}}};

    /*            reflect
     *           +-------+
     *           |       |
     *   reflect |       | reflect (x)
     *           +-------+
     *            reflect (y)
     */

    // particle b should reflect at the low y boundary (cell 12)
    Particle b{{1.0, 1.0, 0}, {0, 1.0, 0}, 1, 1, 5, 1, false};

    container.addParticle(b);

    // deltaT needs to be small enough for reflecting
    double deltaT = 0.0002;
    double epsilon = 5;
    double sigma = 1;

    // run simulation step
    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        PositionComputations::updateOldX(container, 1);
        PositionComputations::stoermerVerlet(container, deltaT, 1);
        container.correctCellMembershipAllParticles();

        ForceComputations::resetForces(container, 1);
        ForceComputations::computeLennardJonesPotential(container);
        ForceComputations::computeGhostParticleRepulsion(container, 1);
        VelocityComputations::stoermerVerlet(container, deltaT, 1);
    }

    // check that particle is present
    bool particlePresent = false;
    for (int i = 0; i < container.getMesh().size(); i++) {
        Cell &c = container.getCell(i);
        auto particleIndices = c.getParticlesIndices();
        if (!particleIndices.empty()) {
            EXPECT_EQ(container.getParticles()[particleIndices[0]].getType(), 1);
            particlePresent = true;
        }
    }
    // particle b should still be in the container
    EXPECT_TRUE(particlePresent);
}

/**
 * @brief checks that the particle stays in the mesh when all boundaries are reflecting and the particle
 * moves diagonally onto one corner
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellDiagonalBoundaryTest) {

    ParticleContainerLinkedCell container{{3, 3, 1}, 1, {{reflect, reflect}, {reflect, reflect}, {outflow, outflow}}};

    /*            reflect
     *           +-------+
     *           |       |
     *   reflect |       | reflect (x)
     *           +-------+
     *            reflect (y)
     */

    // particle b should reflect at the corner (high x, high y) boundary
    Particle b{{1.5, 1.5, 0}, {10.0, 10.0, 0}, 1, 1, 5, 1, false};

    container.addParticle(b);

    // deltaT needs to be small enough for reflecting
    double deltaT = 0.0002;
    double epsilon = 5;
    double sigma = 1;

    // run simulation step
    int iterations = 2000;
    for (int i = 0; i < iterations; i++) {
        PositionComputations::updateOldX(container, 1);
        PositionComputations::stoermerVerlet(container, deltaT, 1);
        container.correctCellMembershipAllParticles();

        ForceComputations::resetForces(container, 1);
        ForceComputations::computeLennardJonesPotential(container);
        ForceComputations::computeGhostParticleRepulsion(container, 1);
        VelocityComputations::stoermerVerlet(container, deltaT, 1);
    }

    // check that particle is present
    bool particlePresent = false;
    for (int i = 0; i < container.getMesh().size(); i++) {
        Cell &c = container.getCell(i);
        auto particleIndices = c.getParticlesIndices();
        if (!particleIndices.empty()) {
            EXPECT_EQ(container.getParticles()[particleIndices[0]].getType(), 1);
            particlePresent = true;
        }
    }
    // particle should still be in the container
    EXPECT_TRUE(particlePresent);
}

/**
 * @brief checks if the container is paritioned correctly for multiple threads
 */
TEST_F(ParticleContainerLinkedCellTest, ParticleContainerLinkedCellMeshPartitionTest)  {
    ParticleContainerLinkedCell container{{6, 3, 1}, 1, {{reflect, reflect}, {reflect, reflect}, {outflow, outflow}}};
    container.computeMeshPartition(2);
    auto matrixPair = container.getMeshPartition();
    std::vector<size_t> p1 = {0, 6, 12};
    std::vector<size_t> p2 = {3, 9, 15};
    std::vector<size_t> b1 = {1, 2, 7, 8, 13, 14};
    std::vector<size_t> b2 = {4, 5, 10, 11, 16, 17};
    EXPECT_TRUE(matrixPair.first.size() == 2);
    EXPECT_TRUE(matrixPair.first[0] == p1);
    EXPECT_TRUE(matrixPair.first[1] == p2);
    EXPECT_TRUE(matrixPair.second[0] == b1);
    EXPECT_TRUE(matrixPair.second[1] == b2);
}
