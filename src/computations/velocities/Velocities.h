#include "particle/Particle.h"
#include <vector>

namespace velocities {
/**
 * @brief computes the updated velocities for the given vector of planets
 * in-place
 * @param planets the planets to compute the velocities of
 * @param deltaT the size of timesteps used in the simulation
 */
void stoermerVerlet(std::vector<Particle> &planets, double deltaT);
} // namespace velocities
