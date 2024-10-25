#include "Particle.h"
#include "utils/ArrayUtils.h"
#include <array>
#include <cmath>
#include <functional>

// computes the updated positions for the given vector of planets in-place
// @param planets the planets to compute the positions of
// @param deltaT the size of timesteps used in the simulation
void computePositions(std::vector<Particle> &planets, double deltaT) {
  for (Particle &planet : planets) {
    std::array<double, 3> posFromVel = ArrayUtils::elementWiseScalarOp(
        deltaT, planet.getV(), std::multiplies<>());

    std::array<double, 3> acc = ArrayUtils::elementWiseScalarOp(
        (1.0 / 2 * planet.getM()), planet.getOldF(), std::multiplies<>());

    std::array<double, 3> posFromAcc = ArrayUtils::elementWiseScalarOp(
        std::exp(std::log(deltaT) * 2), acc, std::multiplies<>());

    std::array<double, 3> deltaX =
        ArrayUtils::elementWisePairOp(posFromVel, posFromAcc, std::plus<>());

    planet.setX(
        ArrayUtils::elementWisePairOp(planet.getX(), deltaX, std::plus<>()));
  }
}

// computes the updated velocities for the given vector of planets in-place
// @param planets the planets to compute the velocities of
// @param deltaT the size of timesteps used in the simulation
void computeVelocities(std::vector<Particle> &planets, double deltaT) {
  for (Particle &planet : planets) {
    double coefficient = deltaT / (2 * planet.getM());
    std::array<double, 3> deltaV = ArrayUtils::elementWiseScalarOp(
        coefficient,
        ArrayUtils::elementWisePairOp(planet.getF(), planet.getOldF(),
                                      std::plus<>()),
        std::multiplies<>());
    planet.setV(ArrayUtils::elementWisePairOp(planet.getV(), deltaV, std::plus<>()));
  }
}
