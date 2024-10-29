#include "particle/Particle.h"
#include <vector>

namespace positions {
/**
 * @briefcomputes the updated positions for the given vector of planets in-place
 * @param planets the planets to compute the positions of
 * @param deltaT the size of timesteps used in the simulation
 */
void stoermerVerlet(std::vector<Particle> &planets, double deltaT);
} // namespace positions
