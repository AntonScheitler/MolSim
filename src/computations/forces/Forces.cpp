#include <array>
#include <cmath>
#include <vector>

#include "../../utils/ArrayUtils.h"
#include "particle/Particle.h"

namespace forces {
    void computeGravitySinglePlanet(Particle &planet, std::vector<Particle> &planets);

    void computeGravity(std::vector<Particle> &planets) {
        for (Particle &planet : planets) {
            computeGravitySinglePlanet(planet, planets);
        }
    }

    void computeGravitySinglePlanet(Particle &planet, std::vector<Particle> &planets) {
        std::array<double, 3> newForce = {0, 0, 0};
        planet.setOldF(planet.getF());

        for (Particle &otherPlanet : planets) {
            if (planet == otherPlanet) {
                continue;
            }
                std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp( otherPlanet.getX(), planet.getX(), std::minus<>());
                double coefficient = (planet.getM() * otherPlanet.getM()) / std::pow(ArrayUtils::L2Norm(distanceVector), 3);
                std::array<double, 3> partialNewForce = ArrayUtils::elementWiseScalarOp( coefficient, distanceVector, std::multiplies<>());
                newForce = ArrayUtils::elementWisePairOp(newForce, partialNewForce, std::plus<>());
        }

        planet.setF(newForce);
    }
}
