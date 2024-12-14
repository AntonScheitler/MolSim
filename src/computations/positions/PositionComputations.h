#include "particle/container/ParticleContainer.h"

/**
 * @brief a class wrapping position-computing functions and their utility functions
 */
class PositionComputations {
    public:
        /**
         * @brief computes the updated positions for the given particleContainer of planets in-place
         * @param particles_arg the particles to compute the positions of
         * @param deltaT_arg the timestep size used by the simulation
         */
        static void stoermerVerlet(ParticleContainer& particles, double deltaT);
        /**
         * @brief sets the oldX parameter of all given particles to their new x param value
         * @param particles the container containing the particles
         */
        static void updateOldX(ParticleContainer &particles);
};
