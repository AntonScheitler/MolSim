#include "./TemperatureComputations.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "spdlogConfig.h"
#include <functional>


void TemperatureComputations::initTemp(ParticleContainer &particles, double initialTemp, int dimensions) {
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;
        double factor = sqrt(initialTemp / particle.getM());
        std::array<double, 3> velocity = maxwellBoltzmannDistributedVelocity(factor, dimensions);
        particle.setV(velocity);
        SPDLOG_DEBUG("init v: {0}, {1}, {2}", velocity[0], velocity[1], velocity[2]);
    }
}

double TemperatureComputations::calculateVelocityScalingFactor(double currentTemp, double targetTemp,
                                                               double maxDeltaTemp) {
    double newTemp;
    if (targetTemp > currentTemp) {
        newTemp = std::min(currentTemp + maxDeltaTemp, targetTemp);
    } else {
        newTemp = std::max(currentTemp - maxDeltaTemp, targetTemp);
    }

    // temperature scaling factor
    return currentTemp == 0 ? 1 : sqrt(newTemp / currentTemp);
}

double TemperatureComputations::calculateCurrentSystemTemp(ParticleContainer &particles, int dimensions) {
    double temp = 0;
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;

        std::array<double, 3> vSquared = ArrayUtils::elementWisePairOp(particle.getV(), particle.getV(),
                                                                       std::multiplies<>());
        double vScalarProduct = vSquared[0] + vSquared[1] + vSquared[2];
        temp += particle.getM() * vScalarProduct;
    }
    temp /= particles.size() * dimensions;
    SPDLOG_DEBUG("current system temp: {0}", temp);
    return temp;
}


void TemperatureComputations::updateTemp(ParticleContainer &particles, double targetTemp, double maxDeltaTemp,
                                         int dimensions) {
    double currentTemp = calculateCurrentSystemTemp(particles, dimensions);


    // temperature scaling factor
    double beta = calculateVelocityScalingFactor(currentTemp, targetTemp, maxDeltaTemp);

    // update all velocities of all particles
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;

        std::array<double, 3> newV = ArrayUtils::elementWiseScalarOp(beta, particle.getV(), std::multiplies<>());
        particle.setV(newV);
        SPDLOG_DEBUG("new v: {0}, {1}, {2}", newV[0], newV[1], newV[2]);
    }
}

double TemperatureComputations::calculateCurrentSystemTempV2(ParticleContainer &particles, std::array<double, 3> avgV,
                                                             int dimensions) {
    double currentTemp = 0;
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;

        std::array<double, 3> thermalMotion = ArrayUtils::elementWisePairOp(particle.getV(), avgV, std::minus<>());
        std::array<double, 3> vSquared = ArrayUtils::elementWisePairOp(thermalMotion, thermalMotion,
                                                                       std::multiplies<>());
        double vScalarProduct = vSquared[0] + vSquared[1] + vSquared[2];
        currentTemp += particle.getM() * vScalarProduct;
    }
    currentTemp = currentTemp / (particles.size() * dimensions);
    SPDLOG_DEBUG("current system temp: {0}", currentTemp);
    return currentTemp;
}


void TemperatureComputations::updateTempV2(ParticleContainer &particles, double targetTemp, double maxDeltaTemp,
                                           int dimensions) {
    std::array<double, 3> avgV = {0, 0, 0};
    size_t nonFixedParticles = 0;
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;
        avgV = ArrayUtils::elementWisePairOp(avgV, particle.getV(), std::plus<>());
        nonFixedParticles++;
    }
    avgV = ArrayUtils::elementWiseScalarOp(1.0 / nonFixedParticles, avgV, std::multiplies<>());

    double currentTemp = calculateCurrentSystemTempV2(particles, avgV, dimensions);

    double beta = calculateVelocityScalingFactor(currentTemp, targetTemp, maxDeltaTemp);

    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;

        // calculate thermal motion (^v)
        std::array<double, 3> thermalMotion = ArrayUtils::elementWisePairOp(particle.getV(), avgV, std::minus<>());

        // velocity scaling only on thermal motion
        std::array<double, 3> scaledV = ArrayUtils::elementWiseScalarOp(beta, thermalMotion, std::multiplies<>());

        // new total velocity of particle is sum of thermal motion and kinetic motion (average velocity)
        std::array<double, 3> newV = ArrayUtils::elementWisePairOp(avgV, scaledV, std::plus<>());
        particle.setV(newV);
        SPDLOG_DEBUG("new v: {0}, {1}, {2}", newV[0], newV[1], newV[2]);
    }
}


double TemperatureComputations::calculateCurrentSystemTempV3(ParticleContainer &particles, std::array<double, 3> avgV,
                                                             int dimensions) {
    double currentTemp = 0;
    // subtract the mean-velocity of all molecules in y-direction from the y-component of the molecules velocity
    // => speed of flow does not contribute to the temperature
    avgV[0] = 0;
    avgV[2] = 0;
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;

        std::array<double, 3> velMinusMeanVelY = ArrayUtils::elementWisePairOp(particle.getV(), avgV, std::minus<>());
        std::array<double, 3> vSquared = ArrayUtils::elementWisePairOp(velMinusMeanVelY, velMinusMeanVelY,
                                                                       std::multiplies<>());
        double vScalarProduct = vSquared[0] + vSquared[1] + vSquared[2];
        currentTemp += particle.getM() * vScalarProduct;
    }
    currentTemp = currentTemp / (particles.size() * dimensions);
    SPDLOG_DEBUG("current system temp: {0}", currentTemp);
    return currentTemp;
}


void TemperatureComputations::updateTempV3(ParticleContainer &particles, double targetTemp, double maxDeltaTemp,
                                           int dimensions) {
    std::array<double, 3> avgV = {0, 0, 0};
    size_t nonFixedParticles = 0;
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;
        avgV = ArrayUtils::elementWisePairOp(avgV, particle.getV(), std::plus<>());
        nonFixedParticles++;
    }

    avgV = ArrayUtils::elementWiseScalarOp(1.0 / nonFixedParticles, avgV, std::multiplies<>());
    double currentTemp = calculateCurrentSystemTempV3(particles, avgV, dimensions);

    double beta = calculateVelocityScalingFactor(currentTemp, targetTemp, maxDeltaTemp);
    // velocity scaling is only applied to x and z part of velocity
    std::array<double, 3> betaXZ = {beta, 1, beta};

    // velocity scaling of non-fixed molecules
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;

        std::array<double, 3> newV = ArrayUtils::elementWisePairOp(betaXZ, particle.getV(), std::multiplies<>());
        particle.setV(newV);
        SPDLOG_DEBUG("new v: {0}, {1}, {2}", newV[0], newV[1], newV[2]);
    }
}
