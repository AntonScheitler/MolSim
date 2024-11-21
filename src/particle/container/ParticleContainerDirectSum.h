#include <vector>
#include "../Particle.h"
#include "ParticleContainer.h"

#pragma once

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate particles or particle pairs
 */
class ParticleContainerDirectSum : public ParticleContainer {

public:
    /**
     * @brief creates an empty ParticleContainer
     * @return the object of an empty ParticleContainer
     */
    ParticleContainerDirectSum();

    /**
     * @brief wraps a vector in a ParticleContainer
     * @param particlesArg the particle vector to be wrapped
     * @return the object of the ParticleContainer
     */
    explicit ParticleContainerDirectSum(const std::vector<Particle> &particlesArg, double averageVelocityArg);

    void addParticle(const Particle &particle) override;
    int size() override;
    double getAverageVelocity() override;
    void setAverageVelocity(double averageVelocityArg) override;
    Particle& getParticle(int index) override;
    ParticleIterator begin() override;
    ParticleIterator end() override;
    PairParticleIterator beginPairParticle() override;
    PairParticleIterator endPairParticle() override;


protected:
    /**
     * @brief the particles wrapped by this container
     */
    std::vector<Particle> particles;
    /**
     * @brief the average brownian velocity of the particles
     */
    double averageVelocity;
};
