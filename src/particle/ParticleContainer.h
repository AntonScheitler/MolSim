#include <vector>
#include "Particle.h"
#include "ParticleIterator.h"
#include "PairParticleIterator.h"

#pragma once

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate particles or particle pairs
 */
class ParticleContainer {

public:
    /**
     * @brief creates an empty ParticleContainer
     * @return the object of an empty ParticleContainer
     */
    ParticleContainer();

    /**
     * @brief wraps a vector in a ParticleContainer
     * @param particlesArg the particle vector to be wrapped
     * @return the object of the ParticleContainer
     */
    explicit ParticleContainer(const std::vector<Particle> &particlesArg, double averageVelocityArg);

    /**
     * @brief adds a particle to the container
     * @param particle the particle to be added
     */
    void addParticle(const Particle &particle);

    /**
     * @brief returns the number of particles in the ParticleContainer
     * @return the number of particles in the ParticleContainer
     */
    int size();
    /**
     * @brief returns the average velocity that exists due to brownian motion
     * @return the average velocity that exists due to brownian motion
     */
    double getAverageVelocity();
    /**
     * @brief sets the average velocity that exists due to brownian motion
     * @param averageVelocityArg the velocity to set the average velocity to
     */
    void setAverageVelocity(double averageVelocityArg);
    Particle& getParticle(int index);
    /**
     * @brief returns a ParticleIterator which points to the beginning of the ParticleContainer. This makes iteration over
     * separate particles the default when using a for-each loop
     * @return a ParticleIterator which points to the beginning of the ParticleContainer
     */
    ParticleIterator begin();
    /**
     * @brief returns a ParticleIterator which points to the end of the ParticleContainer. This makes iteration over
     * separate particles the default when using a for-each loop
     * @return a ParticleIterator which points to the end of the ParticleContainer
     */
    ParticleIterator end();

    /**
     * @brief returns a PairParticleIterator which points to the first pair of the ParticleContainer
     * @return a PairParticleIterator which points to the first pair of the ParticleContainer
     */
    PairParticleIterator beginPairParticle();

    /**
     * @brief returns a PairParticleIterator which points to the theoretical pair after the last pair of the ParticleContainer
     * @return a PairParticleIterator which points to the theoretical pair after the last pair of the ParticleContainer
     */
    PairParticleIterator endPairParticle();

protected:
    /**
     * the particles wrapped by this container
     */
    std::vector<Particle> particles;
    /**
     * the average brownian velocity of the particles
     */
    double averageVelocity;
};
