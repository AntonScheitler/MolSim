#include <gtest/gtest.h>
#include <utils/ArrayUtils.h>
#include "../../src/particle/container/ParticleContainerLinkedCell.h"
#include "../../src/io/inputReader/ParticleGenerator.h"
#include "../../src/simulation/SimulationData.h"
#include "../../src/simulation/Simulator.h"
#include "../../src/particle/Particle.h"
#include "spdlogConfig.h"

class FixedParticleTest : public testing::Test {
protected:

    ParticleContainerLinkedCell particles;

    FixedParticleTest()
            : particles({10, 10, 10}, 3, {{reflect, reflect}, {reflect, reflect}, {reflect, reflect}}) {}

    void generateCuboidParticles() {

        std::array<double, 3> p = {0.0, 0.0, 0.0};
        std::array<double, 3> v = {0.0, 0.0, 0.0};
        std::array<int, 3> d = {3, 3, 1};
        double mass = 1.0;
        double h = 1.0;
        int type = 0;
        double epsilon = 1.0;
        double sigma = 1.0;


        inputReader::ParticleGenerator::generateCuboid(particles, p, v, d, mass, h, type, epsilon, sigma, false, 3);

        inputReader::ParticleGenerator::generateCuboid(particles, {0,0,1}, v, d, mass, h, type, epsilon, sigma, true, 3);

    }
};

/**
 * @brief checks the values of the position and force of a fixed particle stay the same after multiple iterations
 */
TEST_F(FixedParticleTest, FixedParticlesForcesAndPositionsAreUnchanged) {
    std::unique_ptr<ParticleContainer> particlesBeforePtr = particles.copy();
    auto* particlesBefore = dynamic_cast<ParticleContainerLinkedCell*>(particlesBeforePtr.get());


    SimulationData simulationData = SimulationData();

    simulationData.setThermoVersion(2);
    simulationData.setInitialTemp(40);
    simulationData.setThermoFrequency(2);

    simulationData.setParticles(std::make_unique<ParticleContainerLinkedCell>(particles));

    simulationData.setDeltaT(0.5);
    simulationData.setEndTime(10);

    simulationData.setWriteFrequency(30);


    Simulator simulator{simulationData};
    simulator.simulate();


    for(int i = 0; i < particles.size(); i++){
        if(particles.getParticle(i).isFixed()){
            EXPECT_EQ(particlesBefore->getParticle(i).getF(), particles.getParticle(i).getF());
            EXPECT_EQ(particlesBefore->getParticle(i).getX(), particles.getParticle(i).getX());
        }else{
            EXPECT_NE(particlesBefore->getParticle(i).getF(), particles.getParticle(i).getF());
            EXPECT_NE(particlesBefore->getParticle(i).getX(), particles.getParticle(i).getX());
        }
    }
}

