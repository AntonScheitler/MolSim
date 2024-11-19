#include "../../utils/ArrayUtils.h"
#include "particle/ParticleContainerDirectSum.h"
#include "VelocityComputations.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <array>
#include <functional>
#include "spdlogConfig.h"

void VelocityComputations::stoermerVerlet(ParticleContainer &particles, double deltaT) {
    for (Particle &planet: particles) {
        // velocity calculation based on the Stoermer-Verlet formula
        double coefficient = deltaT / (2 * planet.getM());
        std::array<double, 3> deltaV = ArrayUtils::elementWiseScalarOp(coefficient,
                                                                       ArrayUtils::elementWisePairOp(planet.getF(),
                                                                                                     planet.getOldF(),
                                                                                                     std::plus<>()),
                                                                       std::multiplies<>());
        planet.setV(ArrayUtils::elementWisePairOp(planet.getV(), deltaV, std::plus<>()));
    }
}

void VelocityComputations::applyBrownianMotion(ParticleContainer &particles, double averageVelocity) {
    for (Particle &particle: particles) {
        std::array<double, 3> maxBoltzVelocity = maxwellBoltzmannDistributedVelocity(averageVelocity, 3);
        SPDLOG_DEBUG("maxBoltzVelocity: {0}, {1}, {2}", maxBoltzVelocity[0], maxBoltzVelocity[1], maxBoltzVelocity[2]);
        // add the velocity from the brownian motion to the initial velocity
        particle.setV(ArrayUtils::elementWisePairOp(particle.getV(), maxBoltzVelocity, std::plus<>()));
    }
}
