#include "particle/ParticleContainer.h"

/**
 * @brief a class wrapping force-computing functions and their utility functions
 */
class ForceComputations {
    public:
        /**
         * @brief computes the gravitational forces between the planets and changes them in-place
         * @param particles the planets, for which the gravitational forces need to be computed
         */
        static void computeGravity(ParticleContainer& particles);
        /**
         * @brief updates the old force and resets the current force for all particles
         */
        static void resetForces(ParticleContainer& particles);
};
