#include <array>
#include <cmath>
#include <functional>

#include "../../utils/ArrayUtils.h"
#include "particle/ParticleContainer.h"

namespace forces {
    void computeGravity(ParticleContainer &planets) {
        for (Particle& planet: planets) {
            planet.setOldF(planet.getF());
            planet.setF({0, 0, 0});
        }

        for (auto it = planets.beginPairParticle(); it != planets.endPairParticle(); ++it) {
            std::pair<Particle&, Particle&> pair = *it;
            std::array<double, 3> newForce = {0, 0, 0};

            std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.second.getX(), pair.first.getX(), std::minus<>());
            double coefficient = (pair.first.getM() * pair.second.getM()) / std::pow(ArrayUtils::L2Norm(distanceVector), 3);
            newForce = ArrayUtils::elementWiseScalarOp(coefficient, distanceVector, std::multiplies<>());
            pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), newForce, std::plus<>()));
            std::array<double, 3> revNewForce = ArrayUtils::elementWiseScalarOp(-1, newForce, std::multiplies<>());
            pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revNewForce, std::plus<>()));
        }
    }
}
