#include "../../utils/ArrayUtils.h"
#include "particle/container/ParticleContainer.h"
#include "VelocityComputations.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <array>
#include "spdlogConfig.h"
#include <omp.h>

void VelocityComputations::stoermerVerlet(ParticleContainer &particles, double deltaT) {
    #pragma omp parallel for
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

void VelocityComputations::applyBrownianMotion(ParticleContainer &particles, double averageVelocity, int dimensions) {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); i++) {
        Particle &particle = particles.getParticle(i);
        if (particle.isFixed() || !(particle.getActive())) continue;
        std::array<double, 3> maxBoltzVelocity = maxwellBoltzmannDistributedVelocity(averageVelocity, dimensions);
        SPDLOG_DEBUG("maxBoltzVelocity: {0}, {1}, {2}", maxBoltzVelocity[0], maxBoltzVelocity[1], maxBoltzVelocity[2]);
        // add the velocity from the brownian motion to the initial velocity
        particle.setV(ArrayUtils::elementWisePairOp(particle.getV(), maxBoltzVelocity, std::plus<>()));
    }
}

