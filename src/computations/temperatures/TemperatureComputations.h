#pragma once
#include "particle/container/ParticleContainer.h"

/**
 * @brief a class wrapping position-computing functions and their utility functions
 */
class TemperatureComputations {
    public:
        static void updateTemp(ParticleContainer& particles, double targetTemp, double maxDeltaTemp);
        static void initTemp(ParticleContainer& particles, double initialTemp);
    private:
        static double calculateCurrentSystemTemp(ParticleContainer& particles);
};
