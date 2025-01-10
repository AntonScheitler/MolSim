
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

        inputReader::ParticleGenerator::generateCuboid(particles, p, v, d, mass, h, type, epsilon, sigma);
    }
};

// Test bottom left corner
TEST_F(ParticleNeighborsTest, BottomLeftCornerStoresNeighborsCorrectly) {

    generateCuboidParticles();



    Particle p = particles.getParticleAt(0);
    std::cout << "Particle at index 0 exists." << std::endl;

    std::cout << "Particle has bottom." << p.getNeighborIdx(bottom) << std::endl;

    EXPECT_EQ(p.getNeighborIdx(left), -1) << "Left neighbor check failed.";
    EXPECT_EQ(p.getNeighborIdx(bottom), -1) << "Bottom neighbor check failed.";
    EXPECT_EQ(p.getNeighborIdx(right), 1) << "Right neighbor check failed.";
    EXPECT_EQ(p.getNeighborIdx(top), 3) << "Top neighbor check failed.";


}

