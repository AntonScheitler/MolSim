#pragma once
#include "particle/container/ParticleContainerLinkedCell.h"

class VelocityDensityProfile {
    public:
        /**
         * @brief determines the velocity/density profile of the particles and write the result to a csv 
         * @param particles the particles to determine the profile of
         * @param numBins the number of bins to create
         */
        static void determineProfile(ParticleContainerLinkedCell& particles, size_t numBins);
        struct binInfo {
            std::array<double, 3> sumVelocities;
            size_t numParticles;
        };
};
