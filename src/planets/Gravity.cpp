#include <array>
#include <cmath>
#include <vector>

#include "particle/Particle.h"
#include "../utils/ArrayUtils.h"

void computeGravitySinglePlanet(long unsigned int planetIdx,
                                std::vector<Particle> &planets);

void computeGravity(std::vector<Particle> &planets) {
  for (long unsigned int planetIdx = 0; planetIdx < planets.size(); planetIdx++) {
    computeGravitySinglePlanet(planetIdx, planets);
  }
}

void computeGravitySinglePlanet(long unsigned int planetIdx, std::vector<Particle> &planets) {
  std::array<double, 3> newForce = {0, 0, 0};
  Particle planet = planets[planetIdx];
  planets[planetIdx].setOldF(planet.getF());

  for (long unsigned int otherPlanetIdx = 0; otherPlanetIdx < planets.size(); otherPlanetIdx++) {
    if (planetIdx != otherPlanetIdx) {
      Particle otherPlanet = planets[otherPlanetIdx];
      std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp( otherPlanet.getX(), planet.getX(), std::minus<>());
      double coefficient = (planet.getM() * otherPlanet.getM()) / std::pow(ArrayUtils::L2Norm(distanceVector), 3);
      std::array<double, 3> partialNewForce = ArrayUtils::elementWiseScalarOp( coefficient, distanceVector, std::multiplies<>());
      newForce = ArrayUtils::elementWisePairOp(newForce, partialNewForce, std::plus<>());
    }
  }

  planets[planetIdx].setF(newForce);
}
