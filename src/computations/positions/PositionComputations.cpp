#include "../../utils/ArrayUtils.h"
#include "particle/Particle.h"
#include "PositionComputations.h"

void PositionComputations::stoermerVerlet(ParticleContainer& particles, double deltaT) {
    for (Particle &particle : particles) {
        // position calculation based on the Stoermer-Verlet formula
        std::array<double, 3> posFromVel = ArrayUtils::elementWiseScalarOp( deltaT, particle.getV(), std::multiplies<>());
        std::array<double, 3> acc = ArrayUtils::elementWiseScalarOp( (1.0 / (2 * particle.getM())), particle.getF(), std::multiplies<>());
        std::array<double, 3> posFromAcc = ArrayUtils::elementWiseScalarOp( std::pow(deltaT, 2), acc, std::multiplies<>());
        std::array<double, 3> deltaX = ArrayUtils::elementWisePairOp(posFromVel, posFromAcc, std::plus<>());
        particle.setX( ArrayUtils::elementWisePairOp(particle.getX(), deltaX, std::plus<>()));
    }
}
