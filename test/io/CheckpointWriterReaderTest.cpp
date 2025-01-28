#include <gtest/gtest.h>
#include <utils/ArrayUtils.h>
#include "../../src/simulation/SimulationData.h"
#include "../../src/particle/container/ParticleContainerLinkedCell.h"
#include "../../src/io/outputWriter/CheckpointWriter.h"
#include "../../src/io/inputReader/CheckpointReader.h"
#include "../../src/io/inputReader/ParticleGenerator.h"
#include "../../src/particle/Particle.h"
#include "spdlogConfig.h"
#include "iostream"

class CheckpointWriterReaderTest : public testing::Test {
protected:

    ParticleContainerLinkedCell particles;

    CheckpointWriterReaderTest()
            : particles({10, 10, 10}, 3, {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}) {}

    void generateParticles() {

        std::array<double, 3> p = {0.0, 0.0, 0.0};
        std::array<double, 3> v = {0.0, 0.0, 0.0};
        std::array<int, 3> d = {3, 3, 1};
        double mass = 1.0;
        double h = 1.0;
        int type = 0;
        double epsilon = 1.0;
        double sigma = 1.0;

        inputReader::ParticleGenerator::generateCuboid(particles, p, v, d, mass, h, type, epsilon, sigma, false);

        p = {0.0, 15, 0.0};
        v = {0.0, -2, 0.0};
        int r = 3;
        type = 1;
        epsilon = 1.0;
        sigma = 1.0;
        //inputReader::ParticleGenerator::generateDisc(particles, p, v, r, mass, h, type, epsilon, sigma);
    }
};

// Test bottom left corner
TEST_F(CheckpointWriterReaderTest, ParticleContainerIsTheSameAfterCheckpointing) {

    generateParticles();

    outputWriter::CheckpointWriter checkpointWriter("checkerTest");
    checkpointWriter.plotParticles(particles, 0);

    auto tmpParticles = std::make_unique<ParticleContainerLinkedCell>(
            ParticleContainerLinkedCell{{10, 10, 10}, 3,
                                        {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}});
    SimulationData simulationData = SimulationData();
    simulationData.setParticles(std::move(tmpParticles));
    inputReader::CheckpointReader checkpointReader(simulationData);

    checkpointReader.readCheckpointFile(simulationData, "./checkerTest.txt");

    int i = 0;
    for (Particle& particle : simulationData.getParticles()) {
        EXPECT_TRUE(particle.getM() == particles.getParticle(i).getM());
        EXPECT_TRUE(particle.getX() == particles.getParticle(i).getX());
        EXPECT_TRUE(particle.getV() == particles.getParticle(i).getV());
        i++;
    }

}




