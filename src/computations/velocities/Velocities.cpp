#include "../../utils/ArrayUtils.h"
#include "particle/Particle.h"
#include <vector>

namespace velocities {
/**
 * @brief computes the updated velocities for the given vector of planets
 * in-place
 * @param planets the planets to compute the velocities of
 * @param deltaT the size of timesteps used in the simulation
 */
void stoermerVerlet(std::vector<Particle> &planets, double deltaT) {
  for (Particle &planet : planets) {
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
