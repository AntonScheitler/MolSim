#pragma once

#include "particle/container/ParticleContainer.h"

/**
 * @brief a class containing temperature calculation functions for the thermostat
 */
class TemperatureComputations {
public:
    /**
     * @brief Initializes the system with Brownian-Motion (to have a non-zero temperature if no initial velocities
     * are set)
     * @param particles the container containing all particles of the system
     * @param initialTemp the initial temperature of the system
     * @param dimensions the number of dimensions the simulation uses (either 2 or 3)
     */
    static void initTemp(ParticleContainer &particles, double initialTemp, int dimensions);

    /**
     * @brief updates the temperature of the system by scaling all particles' velocities with factor beta
     * @param particles the container containing all particles of the system
     * @param targetTemp the target temperature
     * @param maxDeltaTemp the maximum allowed temperature change for one application of the thermostat
     * @param dimensions number of dimensions (2 or 3)
     */
    static void updateTemp(ParticleContainer &particles, double targetTemp, double maxDeltaTemp, int dimensions);

    /**
     * @brief updates the temperature of the system by scaling all particles' velocities using thermal motion
     * (= difference between particle's velocity and all particles' average velocity)
     * (=> ignores total velocity of the fluid)
     * @param particles the container containing all particles of the system
     * @param targetTemp the target temperature
     * @param maxDeltaTemp the maximum allowed temperature change for one application of the thermostat
     * @param dimensions number of dimensions (2 or 3)
     */
    static void updateTempV2(ParticleContainer &particles, double targetTemp, double maxDeltaTemp, int dimensions);

    /**
     * @brief updates the temperature of the system by scaling only the X and Y part of the velocities of the
     * particles
     * @param particles the container containing all particles of the system
     * @param targetTemp the target temperature
     * @param maxDeltaTemp the maximum allowed temperature change for one application of the thermostat
     * @param dimensions number of dimensions (2 or 3)
     */
    static void updateTempV3(ParticleContainer &particles, double targetTemp, double maxDeltaTemp, int dimensions);

    /**
     * @brief calculates the current temperature of the system using kinetic energy
     * @param particles the container containing all particles of the system
     * @param dimensions the number of dimensions the simulation uses (either 2 or 3)
     * @return current temperature of the system
     */
    static double calculateCurrentSystemTemp(ParticleContainer &particles, int dimensions);

    /**
     * @brief calculates the current temperature of the system using kinetic energy and thermal motion (V2)
     * (ignores total velocity of the fluid)
     * @param particles the container containing all particles of the system
     * @param avgV the average velocity of all (non-fixed) particles
     * @param dimensions the number of dimensions the simulation uses (either 2 or 3)
     * @return current temperature of the system
     */
    static double calculateCurrentSystemTempV2(ParticleContainer &particles,
                                               std::array<double, 3> avgV, int dimensions);

    /**
     * @brief calculates the current temperature of the system using kinetic energy and by ignoring the speed of the
     * flow (=> the mean velocity of all particles in y-direction is subtracted from y-component of each particle)
     * @param particles the container containing all particles of the system
     * @param avgV the average velocity of all (non-fixed) particles
     * @param dimensions the number of dimensions the simulation uses (either 2 or 3)
     * @return current temperature of the system
     */
    static double calculateCurrentSystemTempV3(ParticleContainer &particles,
                                               std::array<double, 3> avgV, int dimensions);

private:
    /**
     * @brief calculates the velocity scaling factor beta (for all thermostat versions)
     * @param currentTemp current temperature of the whole system
     * @param targetTemp target temperature of the whole system
     * @param maxDeltaTemp the maximum allowed temperature change for one application of the thermostat
     * @return velocity scaling factor beta
     */
    static double calculateVelocityScalingFactor(double currentTemp, double targetTemp, double maxDeltaTemp);
};
