#include "../../../../src/computations/positions/PositionComputations.h"
#include "../../../../src/computations/temperatures/TemperatureComputations.h"
#include <particle/container/ParticleContainerLinkedCell.h>
#include <simulation/SimulationData.h>
#include <simulation/Simulator.h>
#include <gtest/gtest.h>

/**
 * @brief Tests the temperature computations
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
        simulator.step(i, i);
    }
    simulator.after();

    // check that temp is equal to target temp
    double currentTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles(), 2);
    EXPECT_DOUBLE_EQ(currentTemp, simData.getTargetTemp());
}

/**
 * @brief checks that the system cools down to the target temperature when targetTemp < initialTemp
 */
TEST_F(TemperatureComputationsTest, CoolingTest) {

    simData.setTargetTemp(0);
    Simulator simulator{simData};

    simulator.before();
    int iterations = 100;
    for(int i = 0; i < iterations; i++) {
        simulator.step(i, i);
    }
    simulator.after();

    // check that temp is equal to target temp
    double currentTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles(), 2);
    EXPECT_DOUBLE_EQ(currentTemp, simData.getTargetTemp());
}

/**
 * @brief checks that the system holds its temperature up to the target temperature when targetTemp = initialTemp
 */
TEST_F(TemperatureComputationsTest, HoldingTempTest) {

    simData.setTargetTemp(simData.getInitialTemp());
    Simulator simulator{simData};

    simulator.before();
    int iterations = 100;
    for(int i = 0; i < iterations; i++) {
        simulator.step(i, i);
    }
    simulator.after();

    // check that temp is equal to target temp
    double currentTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles(), 2);
    EXPECT_DOUBLE_EQ(currentTemp, simData.getTargetTemp());
}

/**
 * @brief checks that a system with no initial velocities gets initialized by Brownian motion to have a non-zero
 * temperature
 */
TEST_F(TemperatureComputationsTest, BrownianMotionInitialVelocityTest) {
    Simulator simulator{simData};

    double initialTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles(), 2);
    EXPECT_DOUBLE_EQ(initialTemp, 0);

    // this applies the brownian motion as initial velocities (by calling initTemp)
    simulator.before();

    double initializedTemp = TemperatureComputations::calculateCurrentSystemTemp(simData.getParticles(), 2);
    EXPECT_NE(initializedTemp, 0);
}

/**
 * @brief a simple test testing that the thermo update of the thermostat v2 works correctly.
 * the average velocity is just the velocity of that one single particle so the thermal motion is 0
 */
TEST_F(TemperatureComputationsTest, ThermoV2SimpleTest) {
    ParticleContainerLinkedCell container{{10, 10, 10}, 10,
                                              {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};

    Particle p1{{2, 2, 2}, {10, 10, 10}, 1, 0};

    container.addParticle(p1);

    simData = SimulationData{};
    simData.setParticles(std::make_unique<ParticleContainerLinkedCell>(container));

    simData.activateThermostat();
    simData.setThermoVersion(2);
    simData.setInitialTemp(10);
    simData.setMaxDeltaTemp(1);
    simData.setNumberDimensions(3);

    TemperatureComputations::updateTempV2(simData.getParticles(), simData.getTargetTemp(), simData.getMaxDeltaTemp(), simData.getNumberDimensions());

    auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));
    int index = containerLinkedCell->continuousCoordsToIndex({2, 2, 2});
    EXPECT_EQ(containerLinkedCell->getParticle(index).getV()[0], 10);
    EXPECT_EQ(containerLinkedCell->getParticle(index).getV()[1], 10);
    EXPECT_EQ(containerLinkedCell->getParticle(index).getV()[2], 10);
}

/**
 * @brief a simple test testing that the thermo update of the thermostat v2 works correctly.
 * two particles with average velocity 15
 * the first particle should get scaled to a lower velocity and the second particle to a higher velocity
 */
TEST_F(TemperatureComputationsTest, ThermoV2Test) {
    ParticleContainerLinkedCell container{{100, 100, 100}, 10,
                                              {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}}};

    Particle p1{{10, 10, 10}, {10, 10, 10}, 1, 0};
    Particle p2{{50, 50, 50}, {20, 20, 20}, 1, 1};

    container.addParticle(p1);
    container.addParticle(p2);

    simData = SimulationData{};
    simData.setParticles(std::make_unique<ParticleContainerLinkedCell>(container));

    simData.activateThermostat();
    simData.setThermoVersion(2);
    simData.setInitialTemp(10);
    simData.setTargetTemp(30);
    simData.setMaxDeltaTemp(10);
    simData.setNumberDimensions(3);

    auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));

    TemperatureComputations::updateTempV2(simData.getParticles(), simData.getTargetTemp(), simData.getMaxDeltaTemp(), simData.getNumberDimensions());

    Particle& a = containerLinkedCell->getParticle(0);
    EXPECT_EQ(a.getType(), 0);
    EXPECT_LT(a.getV()[0], 10);
    EXPECT_LT(a.getV()[1], 10);
    EXPECT_LT(a.getV()[2], 10);

    Particle& b = containerLinkedCell->getParticle(1);
    EXPECT_EQ(b.getType(), 1);
    EXPECT_GT(b.getV()[0], 20);
    EXPECT_GT(b.getV()[1], 20);
    EXPECT_GT(b.getV()[2], 20);
}
