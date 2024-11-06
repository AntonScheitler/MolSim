#include "particle/ParticleContainer.h"

/**
 * @brief a class wrapping position-computing functions and their utility
 * functions
 */
class VelocityComputations {
    public:
        /**
         * @brief computes the updated velocities for the ParticleContainer of planets
         * in-place
         * @param particles_arg the particles to compute the velocities of
         * @param deltaT_arg the timestep size used by the simulation
         */
        static void stoermerVerlet(ParticleContainer &particles, double deltaT);

    private:
};
