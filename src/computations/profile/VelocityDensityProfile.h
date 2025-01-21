#include "particle/container/ParticleContainerLinkedCell.h"

class VelocityDensityProfile {
    public:
        /**
         * @brief determines the velocity/density profile of the particles and write the result to a csv 
         * @param particles the particles to determine the profile of
         */
        static void determineProfile(ParticleContainerLinkedCell& particles);
    private:
        size_t N = 50;
};
