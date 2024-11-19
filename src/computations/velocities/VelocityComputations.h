#include "particle/ParticleContainerDirectSum.h"

/**
 * @brief a class wrapping position-computing functions and their utility functions
 */
class VelocityComputations {
    public:
        /**
         * @brief computes the updated velocities for the ParticleContainer of planets
         * in-place
         * @param particles the particles to compute the velocities of
         * @param deltaT the timestep size used by the simulation
         */
        static void stoermerVerlet(ParticleContainer &particles, double deltaT);

        /**
         * @brief applies an initial velocity to all particles based on the Brownian motion
         * @param particles the particles to apply the velocity to
         * @param averageVelocity the average velocity of the particles
         */
        static void applyBrownianMotion(ParticleContainer &particles, double averageVelocity);

    private:
};
