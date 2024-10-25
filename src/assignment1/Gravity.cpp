#include <array>
#include <cmath>
#include <vector>

#include "../Particle.h"
#include "../utils/ArrayUtils.h"

void computeGravitySinglePlanet(int planetIdx, std::vector<Particle>& planets);

void computeGravity(std::vector<Particle>& planets) {
    for (int planetIdx = 0; planetIdx < planets.size(); planetIdx++) {
        computeGravitySinglePlanet(planetIdx, planets);
    }  
}

void computeGravitySinglePlanet(int planetIdx, std::vector<Particle>& planets) {
    std::array<double, 3> newForce;
    Particle planet = planets[planetIdx];  

    for (int otherPlanetIdx = 0; otherPlanetIdx < planets.size(); otherPlanetIdx++) {
        if (planetIdx == otherPlanetIdx) {
            continue;
        }
        Particle otherPlanet = planets[otherPlanetIdx];
        std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(otherPlanet.getX(), planet.getX(), std::minus<>());
        // todo better way to take exponential
        double coefficient = (planet.getM() * otherPlanet.getM()) / std::exp(std::log(ArrayUtils::L2Norm(distanceVector)) * 3);
        std::array<double, 3> partialNewForce =  ArrayUtils::elementWiseScalarOp(coefficient, distanceVector, std::multiplies<>());
        newForce = ArrayUtils::elementWisePairOp(newForce, partialNewForce, std::plus<>());
    }

    planets[planetIdx].setF(newForce);
}