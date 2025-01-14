
#include <cstddef>
#include <gtest/gtest.h>
#include <unordered_set>
#include <utility>
#include <utils/ArrayUtils.h>
#include "../../src/particle/container/ParticleContainerLinkedCell.h"
#include "../../src/io/inputReader/ParticleGenerator.h"
#include "../../src/particle/Particle.h"
#include "spdlogConfig.h"

class ParticleNeighborsTest : public testing::Test {
protected:

    ParticleContainerLinkedCell particles;

    ParticleNeighborsTest()
            : particles({10, 10, 10}, 3, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}) {}


    void generateCuboidParticles() {

        std::array<double, 3> p = {0.0, 0.0, 0.0};
        std::array<double, 3> v = {0.0, 0.0, 0.0};
        std::array<int, 3> d = {3, 3, 1};
        double mass = 1.0;
        double h = 1.0;
        int type = 0;
        double epsilon = 1.0;
        double sigma = 1.0;

        inputReader::ParticleGenerator::generateCuboid(particles, p, v, d, mass, h, type, epsilon, sigma, false);
    }
};

// Test bottom left corner
TEST_F(ParticleNeighborsTest, BottomLeftCornerStoresNeighborsCorrectly) {

    generateCuboidParticles();



    Particle p = particles.getParticleAt(0);

    EXPECT_EQ(p.getNeighborIdx(left), -1);
    EXPECT_EQ(p.getNeighborIdx(bottom), -1);
    EXPECT_EQ(p.getNeighborIdx(right), 1);
    EXPECT_EQ(p.getNeighborIdx(top), 3);

    EXPECT_EQ(p.getDiagNeighborIdx(bottomLeft), -1);
    EXPECT_EQ(p.getDiagNeighborIdx(bottomRight), -1);
    EXPECT_EQ(p.getDiagNeighborIdx(topLeft), -1);
    EXPECT_EQ(p.getDiagNeighborIdx(topRight), 4);



}

// Test middle
TEST_F(ParticleNeighborsTest, MiddleStoresNeighborsCorrectly) {

generateCuboidParticles();



Particle p = particles.getParticleAt(4);

EXPECT_EQ(p.getNeighborIdx(left), 3);
EXPECT_EQ(p.getNeighborIdx(bottom), 1);
EXPECT_EQ(p.getNeighborIdx(right), 5);
EXPECT_EQ(p.getNeighborIdx(top), 7);

EXPECT_EQ(p.getDiagNeighborIdx(bottomLeft), 0);
EXPECT_EQ(p.getDiagNeighborIdx(bottomRight), 2);
EXPECT_EQ(p.getDiagNeighborIdx(topLeft), 6);
EXPECT_EQ(p.getDiagNeighborIdx(topRight), 8);



}

//Test upper middle
TEST_F(ParticleNeighborsTest, UpperMiddleStoresNeighborsCorrectly) {

generateCuboidParticles();



Particle p = particles.getParticleAt(7);

EXPECT_EQ(p.getNeighborIdx(left), 6);
EXPECT_EQ(p.getNeighborIdx(bottom), 4);
EXPECT_EQ(p.getNeighborIdx(right), 8);
EXPECT_EQ(p.getNeighborIdx(top), -1);

EXPECT_EQ(p.getDiagNeighborIdx(bottomLeft), 3);
EXPECT_EQ(p.getDiagNeighborIdx(bottomRight), 5);
EXPECT_EQ(p.getDiagNeighborIdx(topLeft), -1);
EXPECT_EQ(p.getDiagNeighborIdx(topRight), -1);



}