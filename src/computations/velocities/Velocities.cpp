#include "../../utils/ArrayUtils.h"
#include "particle/ParticleContainer.h"

namespace velocities {

    void stoermerVerlet(ParticleContainer &planets, double deltaT) {
        for (Particle &planet: planets) {
            double coefficient = deltaT / (2 * planet.getM());
            std::array<double, 3> deltaV = ArrayUtils::elementWiseScalarOp(
                    coefficient,
                    ArrayUtils::elementWisePairOp(planet.getF(), planet.getOldF(),
                        std::plus<>()),
                    std::multiplies<>());
            planet.setV(
                    ArrayUtils::elementWisePairOp(planet.getV(), deltaV, std::plus<>()));
        }
    }
} // namespace velocities
