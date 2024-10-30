#include "../../utils/ArrayUtils.h"
#include "particle/Particle.h"
#include "particle/ParticleContainer.h"

namespace positions {

    void stoermerVerlet(ParticleContainer &planets, double deltaT) {
        for (Particle &planet : planets) {
            // position calculation based on the Stoermer-Verlet formula
            std::array<double, 3> posFromVel = ArrayUtils::elementWiseScalarOp( deltaT, planet.getV(), std::multiplies<>());
            std::array<double, 3> acc = ArrayUtils::elementWiseScalarOp( (1.0 / (2 * planet.getM())), planet.getOldF(), std::multiplies<>());
            std::array<double, 3> posFromAcc = ArrayUtils::elementWiseScalarOp( std::pow(deltaT, 2), acc, std::multiplies<>());
            std::array<double, 3> deltaX = ArrayUtils::elementWisePairOp(posFromVel, posFromAcc, std::plus<>());
            planet.setX( ArrayUtils::elementWisePairOp(planet.getX(), deltaX, std::plus<>()));
        }
    }
} // namespace positions
