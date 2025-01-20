#include "./TemperatureComputations.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "spdlogConfig.h"
#include <functional>


void TemperatureComputations::initTemp(ParticleContainer& particles, double initialTemp, int dimensions) {
    for(auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;
        double factor = sqrt(initialTemp / particle.getM());
        std::array<double, 3> velocity = maxwellBoltzmannDistributedVelocity(factor, dimensions);
        particle.setV(velocity);
        SPDLOG_DEBUG("init v: {0}, {1}, {2}", velocity[0], velocity[1], velocity[2]);
    }
}

double TemperatureComputations::calculateCurrentSystemTemp(ParticleContainer &particles) {

    double temp = 0;
    for(auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;

        std::array<double, 3> vSquared = ArrayUtils::elementWisePairOp(particle.getV(), particle.getV(), std::multiplies<>());
        double vScalarProduct = vSquared[0] + vSquared[1] + vSquared[2];
        temp += particle.getM() * vScalarProduct;
    }
    temp = temp / (particles.size() * 2); //TODO: number of dimensions instead of 2
    SPDLOG_DEBUG("current system temp: {0}", temp);
    return temp;
}

void TemperatureComputations::updateTemp(ParticleContainer& particles, double targetTemp, double maxDeltaTemp) {

    double currentTemp = calculateCurrentSystemTemp(particles);

    double newTemp;
    if(targetTemp > currentTemp) {
        newTemp = std::min(currentTemp + maxDeltaTemp, targetTemp);
    } else {
        newTemp = std::max(currentTemp - maxDeltaTemp, targetTemp);
    }

    // temperature scaling fact
    double beta = currentTemp == 0 ? 1 : sqrt(newTemp / currentTemp);

    // update all velocities of all particles
    for(auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;

        std::array<double, 3> newV = ArrayUtils::elementWiseScalarOp(beta, particle.getV(), std::multiplies<>());
        particle.setV(newV);
        SPDLOG_DEBUG("new v: {0}, {1}, {2}", newV[0], newV[1], newV[2]);
    }
}


void TemperatureComputations::updateTempV2(ParticleContainer& particles, double targetTemp, double maxDeltaTemp) {
    std::array<double, 3> avgV = {0, 0, 0};
    size_t nonFixedParticles = 0;
    for(auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;
        avgV = ArrayUtils::elementWisePairOp(avgV, particle.getV(), std::plus<>());
        nonFixedParticles++;
    }
    avgV = ArrayUtils::elementWiseScalarOp(1.0 / nonFixedParticles, avgV, std::multiplies<>());

    double currentTemp = 0;
    for(auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;

        std::array<double, 3> thermalMotion = ArrayUtils::elementWisePairOp(particle.getV(), avgV, std::minus<>());
        std::array<double, 3> vSquared = ArrayUtils::elementWisePairOp(thermalMotion, thermalMotion, std::multiplies<>());
        double vScalarProduct = vSquared[0] + vSquared[1] + vSquared[2];
        currentTemp += particle.getM() * vScalarProduct;
    }
    currentTemp = currentTemp / (particles.size() * 2); //TODO: number of dimensions instead of 2
    SPDLOG_DEBUG("current system temp: {0}", currentTemp);

    double newTemp;
    if(targetTemp > currentTemp) {
        newTemp = std::min(currentTemp + maxDeltaTemp, targetTemp);
    } else {
        newTemp = std::max(currentTemp - maxDeltaTemp, targetTemp);
    }

    // temperature scaling fact
    double beta = currentTemp == 0 ? 1 : sqrt(newTemp / currentTemp);

    for(auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;

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
