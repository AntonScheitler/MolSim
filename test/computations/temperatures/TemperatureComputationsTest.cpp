#include "../../../../src/computations/positions/PositionComputations.h"
#include "../../../../src/computations/temperatures/TemperatureComputations.h"
#include <particle/container/ParticleContainerLinkedCell.h>
#include <simulation/SimulationData.h>
#include <simulation/Simulator.h>
#include <gtest/gtest.h>

/**
 * @brief Tests the position computations (src/computations/positions) via analytically calculated examples
 */
class TemperatureComputationsTest : public testing::Test {
protected:

    SimulationData simData;

    // executed before each test
    void SetUp() override {

        ParticleContainerLinkedCell container{{10, 10, 10}, 11,
                                              {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};

        Particle p{{5, 5, 5}, {0, 0, 0}, 1, 0};
        container.addParticle(p);

        simData = SimulationData{};
        simData.setParticles(std::make_unique<ParticleContainerLinkedCell>(container));

        simData.setAverageVelocity(0.1);
        simData.activateThermostat();
        simData.setInitialTemp(10);
        simData.setThermoFrequency(2); // every 2nd iteration the thermostat is updated
        simData.setMaxDeltaTemp(1);
        // change target temp in each test
    }
};

/*
 * tests:
 * heating: one particle, init temp, target temp, small n, simulate iterations, check that target temp is reached
 * cooling: same but with targetT < initT
 * holding temperature: not target temp, iterations, check that temp is same
 *
 */

/**
 * @brief checks that the system heats up to the target temperature when targetTemp > initialTemp
 */
TEST_F(TemperatureComputationsTest, HeatingTest) {

    simData.setTargetTemp(20);
    Simulator simulator{simData};

    simulator.before();
    int iterations = 100;
    for(int i = 0; i < iterations; i++) {
        simulator.step(i);
    }
    simulator.after();

    // check that temp is equal to target temp
    double currentTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles());
    EXPECT_DOUBLE_EQ(currentTemp, simData.getTargetTemp());
}

/**
 * @brief checks that the system heats up to the target temperature when targetTemp > initialTemp
 */
TEST_F(TemperatureComputationsTest, CoolingTest) {

    simData.setTargetTemp(0);
    Simulator simulator{simData};

    simulator.before();
    int iterations = 100;
    for(int i = 0; i < iterations; i++) {
        simulator.step(i);
    }
    simulator.after();

    // check that temp is equal to target temp
    double currentTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles());
    EXPECT_DOUBLE_EQ(currentTemp, simData.getTargetTemp());
}

/**
 * @brief checks that the system heats up to the target temperature when targetTemp > initialTemp
 */
TEST_F(TemperatureComputationsTest, HoldingTempTest) {

    simData.setTargetTemp(simData.getInitialTemp());
    Simulator simulator{simData};

    simulator.before();
    int iterations = 100;
    for(int i = 0; i < iterations; i++) {
        simulator.step(i);
    }
    simulator.after();

    // check that temp is equal to target temp
    double currentTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles());
    EXPECT_DOUBLE_EQ(currentTemp, simData.getTargetTemp());

    //TODO: run simulation loop with bench=true instead of copying loop
}