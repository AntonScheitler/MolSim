#include "../../../../src/computations/forces/ForceComputations.h"
#include <gtest/gtest.h>
#include "../../../src/spdlogConfig.h"
#include "../../../src/particle/container/ParticleContainerDirectSum.h"
#include "particle/container/ParticleContainerLinkedCell.h"

/**
 * @brief Tests the force computations (src/computations/forces) via analytically calculated examples
 */
class ForceComputationsTest : public testing::Test {
protected:
    ParticleContainerDirectSum particles;

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

        a.setSigma(1);
        b.setSigma(1);

        a.setEpsilon(5);
        b.setEpsilon(5);

        particles = ParticleContainerDirectSum({std::vector<Particle>{a, b}});
        SPDLOG_INFO("ForceComputationsTest setUp done");
    }
};


/**
 * @brief checks if the lennard jones force calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(ForceComputationsTest, LennardJonesForceCalcTest) {
    ForceComputations::computeLennardJonesPotential(particles);

    // the analytical solution F_ij calculates the force for j (for i is inverted)
    // particle a
    EXPECT_NEAR(particles.getParticle(0).getF()[0], 0.000285767, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[1], -0.000142883, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[2], -0.000285767, 0.00000001);
    // particle b
    EXPECT_NEAR(particles.getParticle(1).getF()[0], -0.000285767, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[1], 0.000142883, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[2], 0.000285767, 0.00000001);
}

/**
 * @brief checks if the stoermer verlet gravity force calculation is equivalent to the analytical solution
 * (modulo rounding/discretization errors)
 */
TEST_F(ForceComputationsTest, GravityCalcTest) {
    ForceComputations::computeGravity(particles);

    // the analytical solution F_ij calculates the force for j (for i is inverted)
    // particle a
    EXPECT_NEAR(particles.getParticle(0).getF()[0], 0.111111111, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[1], -0.055555555, 0.00000001);
    EXPECT_NEAR(particles.getParticle(0).getF()[2], -0.111111111, 0.00000001);
    // particle b
    EXPECT_NEAR(particles.getParticle(1).getF()[0], -0.111111111, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[1], 0.055555555, 0.00000001);
    EXPECT_NEAR(particles.getParticle(1).getF()[2], 0.111111111, 0.00000001);
}


TEST_F(ForceComputationsTest, HarmonicPotentialDirectNeighborTest) {
    ParticleContainerLinkedCell container{{2, 1, 1}, 1, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};
    // create first neighbor
    Particle first = {{0.5, 0, 0}, {0, 0, 0}, 1, 1, 1, 1, false};
    first.addNeighborIdx(-1);
    first.addNeighborIdx(-1);
    first.addNeighborIdx(1);
    first.addNeighborIdx(-1);

    first.addDiagNeighborIdx(-1);
    first.addDiagNeighborIdx(-1);
    first.addDiagNeighborIdx(-1);
    first.addDiagNeighborIdx(-1);

    // create second neighbor
    Particle second = {{1.5, 0, 0}, {0, 0, 0}, 1, 1, 1, 1, false};
    second.addNeighborIdx(-1);
    second.addNeighborIdx(0);
    second.addNeighborIdx(-1);
    second.addNeighborIdx(-1);

    second.addDiagNeighborIdx(-1);
    second.addDiagNeighborIdx(-1);
    second.addDiagNeighborIdx(-1);
    second.addDiagNeighborIdx(-1);

    // add particles to the container
    container.addParticle(first);
    container.addParticle(second);

    ForceComputations::computeMembraneNeighborForce(container, 300, 2.2);

    EXPECT_NEAR(container.getParticleAt(0).getF()[0], -384, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(0).getF()[1], 0, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(0).getF()[2], 0, 0.00000001);

    EXPECT_NEAR(container.getParticleAt(1).getF()[0], 384, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(1).getF()[1], 0, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(1).getF()[2], 0, 0.00000001);
}

TEST_F(ForceComputationsTest, HarmonicPotentialDiagonalNeighborTest) {
    ParticleContainerLinkedCell container{{2, 2, 1}, 1, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};
    // create first neighbor
    Particle first = {{0.5, 0.5, 0}, {0, 0, 0}, 1, 1, 1, 1, false};
    first.addNeighborIdx(-1);
    first.addNeighborIdx(-1);
    first.addNeighborIdx(-1);
    first.addNeighborIdx(-1);

    first.addDiagNeighborIdx(-1);
    first.addDiagNeighborIdx(-1);
    first.addDiagNeighborIdx(-1);
    first.addDiagNeighborIdx(1);

    // create second neighbor
    Particle second = {{1.5, 1.5, 0}, {0, 0, 0}, 1, 1, 1, 1, false};
    second.addNeighborIdx(-1);
    second.addNeighborIdx(-1);
    second.addNeighborIdx(-1);
    second.addNeighborIdx(-1);

    second.addDiagNeighborIdx(0);
    second.addDiagNeighborIdx(-1);
    second.addDiagNeighborIdx(-1);
    second.addDiagNeighborIdx(-1);

    // add particles to the container
    container.addParticle(first);
    container.addParticle(second);

    ForceComputations::computeMembraneNeighborForce(container, 300, 2.2);

    EXPECT_NEAR(container.getParticleAt(0).getF()[0], -360, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(0).getF()[1], -360, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(0).getF()[2], 0, 0.00000001);

    EXPECT_NEAR(container.getParticleAt(1).getF()[0], 360, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(1).getF()[1], 360, 0.00000001);
    EXPECT_NEAR(container.getParticleAt(1).getF()[2], 0, 0.00000001);
}

/**
 * @brief checks that the external force (gravity force) is correctly applied to two particles in
 * direct sum ParticleContainer
 */
TEST_F(ForceComputationsTest, ExternalGravityTest) {

    Particle a{0};
    a.setF({1, 2, 3});
    a.setM(6);

    Particle b{0};
    b.setF({3, 2, 1});
    b.setM(1);

    ParticleContainerDirectSum particles = ParticleContainerDirectSum{std::vector{a, b}};

    ForceComputations::addExternalForces(particles, {5, 5, 5});

    // particle a
    EXPECT_DOUBLE_EQ(particles.getParticle(0).getF()[0], 31);
    EXPECT_DOUBLE_EQ(particles.getParticle(0).getF()[1], 32);
    EXPECT_DOUBLE_EQ(particles.getParticle(0).getF()[2], 33);

    // particle b
    EXPECT_DOUBLE_EQ(particles.getParticle(1).getF()[0], 8);
    EXPECT_DOUBLE_EQ(particles.getParticle(1).getF()[1], 7);
    EXPECT_DOUBLE_EQ(particles.getParticle(1).getF()[2], 6);

}
