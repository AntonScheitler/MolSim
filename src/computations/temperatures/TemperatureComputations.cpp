#include "./TemperatureComputations.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"


void TemperatureComputations::initTemp(ParticleContainer& particles, double initialTemp) {
    for (auto it = particles.begin(); *it != *(particles.end()); it->operator++()) {
        Particle& particle = **it;
        double factor = sqrt(initialTemp / particle.getM());
        std::array<double, 3> maxBoltzVelocity = maxwellBoltzmannDistributedVelocity(factor, 3);
        maxBoltzVelocity[2] = 0; // TODO: update this depending on number of dimensions
        particle.setV(maxBoltzVelocity);
    }
}


double TemperatureComputations::calculateCurrentSystemTemp(ParticleContainer &particles) {

    double temp = 0;
    for (auto it = particles.begin(); *it != *(particles.end()); it->operator++()) {
        Particle& particle = **it;

        std::array<double, 3> vSquared = ArrayUtils::elementWisePairOp(particle.getV(), particle.getV(), std::multiplies<>());
        double vScalarProduct = vSquared[0] + vSquared[1] + vSquared[2];
        temp += particle.getM() * vScalarProduct;
    }

    return temp / particles.size() * 2; //TODO: number of dimensions instead of 2
}

void TemperatureComputations::updateTemp(ParticleContainer& particles, double targetTemp, double maxDeltaTemp) {

    double currentTemp = calculateCurrentSystemTemp(particles);
    if(currentTemp == targetTemp) return;

    double newTemp;
    if(targetTemp > currentTemp) {
        if(targetTemp < currentTemp + maxDeltaTemp) {
            newTemp = targetTemp;
        } else {
            newTemp = currentTemp + maxDeltaTemp;
        }
    } else {
        if(targetTemp > currentTemp - maxDeltaTemp) {
            newTemp = targetTemp;
        } else {
            newTemp = currentTemp - maxDeltaTemp;
        }
    }

    // temperature scaling fact
    double beta = sqrt(newTemp / currentTemp);

    // update all velocities of all particles
    for (auto it = particles.begin(); *it != *(particles.end()); it->operator++()) {
        Particle& particle = **it;

        std::array<double, 3> v = particle.getV();
        v[0] *= beta;
        v[1] *= beta;
        v[2] *= beta;
    }
}