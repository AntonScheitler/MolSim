#include "../../utils/ArrayUtils.h"
#include "particle/container/ParticleContainer.h"
#include "VelocityComputations.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <array>
#include "spdlogConfig.h"
#include <omp.h>

void VelocityComputations::stoermerVerlet(ParticleContainer &particles, double deltaT, size_t numThreads) {
    #pragma omp parallel for num_threads(numThreads)
    for (size_t i = 0; i < particles.size(); i++) {
        Particle &particle = particles.getParticle(i);
        if (particle.isFixed() || !(particle.getActive())) continue;
        // velocity calculation based on the Stoermer-Verlet formula
        double coefficient = deltaT / (2 * particle.getM());
        std::array<double, 3> deltaV = ArrayUtils::elementWiseScalarOp(coefficient,
                                                                       ArrayUtils::elementWisePairOp(particle.getF(),
                                                                                                     particle.getOldF(),
                                                                                                     std::plus<>()),
                                                                       std::multiplies<>());
        particle.setV(ArrayUtils::elementWisePairOp(particle.getV(), deltaV, std::plus<>()));
    }
}

void VelocityComputations::applyBrownianMotion2D(ParticleContainer &particles, double averageVelocity, size_t numThreads) {
    #pragma omp parallel for num_threads(numThreads)
    for (size_t i = 0; i < particles.size(); i++) {
        Particle &particle = particles.getParticle(i);
        if (particle.isFixed() || !(particle.getActive())) continue;
        std::array<double, 3> maxBoltzVelocity = maxwellBoltzmannDistributedVelocity(averageVelocity, 3);
        maxBoltzVelocity[2] = 0;
        SPDLOG_DEBUG("maxBoltzVelocity: {0}, {1}, {2}", maxBoltzVelocity[0], maxBoltzVelocity[1], maxBoltzVelocity[2]);
        // add the velocity from the brownian motion to the initial velocity
        particle.setV(ArrayUtils::elementWisePairOp(particle.getV(), maxBoltzVelocity, std::plus<>()));
    }
}

void VelocityComputations::applyBrownianMotion3D(ParticleContainer &particles, double averageVelocity, size_t numThreads) {
    #pragma omp parallel for num_threads(numThreads)
    for (size_t i = 0; i < particles.size(); i++) {
        Particle &particle = particles.getParticle(i);
        if (particle.isFixed() || !(particle.getActive())) continue;
        std::array<double, 3> maxBoltzVelocity = maxwellBoltzmannDistributedVelocity(averageVelocity, 3);
        SPDLOG_DEBUG("maxBoltzVelocity: {0}, {1}, {2}", maxBoltzVelocity[0], maxBoltzVelocity[1], maxBoltzVelocity[2]);
        // add the velocity from the brownian motion to the initial velocity
        particle.setV(ArrayUtils::elementWisePairOp(particle.getV(), maxBoltzVelocity, std::plus<>()));
    }
}
