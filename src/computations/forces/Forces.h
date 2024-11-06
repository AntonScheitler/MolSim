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

    private:
        /**
         * @brief sets the oldF of every particle to that particle's current force. The current force is then reset
         */
        static void resetForces(ParticleContainer& particles);
};
