#include "../../utils/ArrayUtils.h"
#include "particle/ParticleContainer.h"
#include "VelocityComputations.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <array>

void VelocityComputations::stoermerVerlet(ParticleContainer& particles, double deltaT) {
    for (Particle &planet: particles) {
        // velocity calculation based on the Stoermer-Verlet formula
        double coefficient = deltaT / (2 * planet.getM());
        std::array<double, 3> deltaV = ArrayUtils::elementWiseScalarOp( coefficient, ArrayUtils::elementWisePairOp(planet.getF(), planet.getOldF(), std::plus<>()), std::multiplies<>());
        planet.setV( ArrayUtils::elementWisePairOp(planet.getV(), deltaV, std::plus<>()));
    }
}

void VelocityComputations::applyBrownianMotion(ParticleContainer &particles, double averageVelocity) {
    for (Particle& particle : particles) {
        std::array<double, 3> initialVelocity = maxwellBoltzmannDistributedVelocity(averageVelocity, 3);
        particle.setV(initialVelocity);
    }
}
