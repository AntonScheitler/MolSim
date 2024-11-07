#include "particle/ParticleContainer.h"

/**
 * @brief namespace which wraps computation different functions for velocities
 */
namespace velocities {
    /**
     * @brief computes the updated velocities for the ParticleContainer of planets
     * in-place
     * @param planets the planets to compute the velocities of
     * @param deltaT the size of timesteps used in the simulation
     */
    void stoermerVerlet(ParticleContainer &planets, double deltaT);
} // namespace velocities
