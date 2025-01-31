#pragma once
#include "particle/container/ParticleContainerLinkedCell.h"


/**
* @brief This class provides the method to calculate the bin-profiles
*/
class VelocityDensityProfile {
public:
    struct binInfo {
        std::array<double, 3> sumVelocities;
        size_t numParticles;
        double avgDensity;
        std::array<double, 3> avgVelocity;
        double avgVelocityNorm;
    };

    /**
     * @brief determines the velocity/density profile of the particles and write the result to a csv
     * @param particles the particles to determine the profile of
     * @param numBins the number of bins to create
     */
    static std::vector<VelocityDensityProfile::binInfo> determineProfile(
        ParticleContainerLinkedCell &particles, size_t numBins);
};
