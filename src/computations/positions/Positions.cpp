#include "../../utils/ArrayUtils.h"
#include "particle/Particle.h"
#include <vector>

namespace positions {
/**
 * @briefcomputes the updated positions for the given vector of planets in-place
 * @param planets the planets to compute the positions of
 * @param deltaT the size of timesteps used in the simulation
 */
void stoermerVerlet(std::vector<Particle> &planets, double deltaT) {
  for (Particle &planet : planets) {
    std::array<double, 3> posFromVel = ArrayUtils::elementWiseScalarOp(
        deltaT, planet.getV(), std::multiplies<>());

    std::array<double, 3> acc = ArrayUtils::elementWiseScalarOp(
        (1.0 / 2 * planet.getM()), planet.getOldF(), std::multiplies<>());

    std::array<double, 3> posFromAcc = ArrayUtils::elementWiseScalarOp(
        std::pow(deltaT, 2), acc, std::multiplies<>());

    std::array<double, 3> deltaX =
        ArrayUtils::elementWisePairOp(posFromVel, posFromAcc, std::plus<>());

    planet.setX(
        ArrayUtils::elementWisePairOp(planet.getX(), deltaX, std::plus<>()));
  }
}
} // namespace positions
