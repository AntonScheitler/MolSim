#include "../../utils/ArrayUtils.h"
#include "particle/Particle.h"
#include "PositionComputations.h"
#include <omp.h>

void PositionComputations::stoermerVerlet(ParticleContainer& particles, double deltaT) {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); i++) {
        Particle& particle = particles.getParticle(i);
        if(particle.isFixed() || (!particle.getActive())) continue;
        // position calculation based on the Stoermer-Verlet formula
        std::array<double, 3> posFromVel = ArrayUtils::elementWiseScalarOp( deltaT, particle.getV(), std::multiplies<>());
        std::array<double, 3> acc = ArrayUtils::elementWiseScalarOp( (1.0 / (2 * particle.getM())), particle.getF(), std::multiplies<>());
        std::array<double, 3> posFromAcc = ArrayUtils::elementWiseScalarOp( std::pow(deltaT, 2), acc, std::multiplies<>());
        std::array<double, 3> deltaX = ArrayUtils::elementWisePairOp(posFromVel, posFromAcc, std::plus<>());
        particle.setX( ArrayUtils::elementWisePairOp(particle.getX(), deltaX, std::plus<>()));
    }
}

void PositionComputations::updateOldX(ParticleContainer &particles) {
    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); i++) {
        Particle& particle = particles.getParticle(i);
        if (!(particle.getActive())) continue;
        particle.setOldX(particle.getX());
    }
}
