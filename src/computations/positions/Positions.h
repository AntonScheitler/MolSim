#include "particle/ParticleContainer.h"
#include <vector>

namespace positions {
/**
 * @brief computes the updated positions for the given particleContainer of planets in-place
 * @param planets the planets to compute the positions of
 * @param deltaT the size of timesteps used in the simulation
 */
void stoermerVerlet(ParticleContainer &planets, double deltaT);
} // namespace positions
