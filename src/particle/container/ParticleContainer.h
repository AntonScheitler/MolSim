#include "../Particle.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"
#include <memory>

#pragma once

class ParticleContainer {
public:
    ParticleContainer() = default;
    virtual ~ParticleContainer() = default;

    /**
     * @brief adds a particle to the container
     * @param particle the particle to be added
     */
    virtual void addParticle(const Particle &particle) = 0;

    /**
     * @brief returns the number of particles in the ParticleContainer
     * @return the number of particles in the ParticleContainer
     */
    virtual int size() = 0;
//    /**
//     * @brief returns the average velocity that exists due to brownian motion
//     * @return the average velocity that exists due to brownian motion
//     */
//    virtual double getAverageVelocity() = 0;
//    /**
//     * @brief sets the average velocity that exists due to brownian motion
//     * @param averageVelocityArg the velocity to set the average velocity to
//     */
//    virtual void setAverageVelocity(double averageVelocityArg) = 0;

    virtual std::unique_ptr<ParticleIterator> begin() = 0;
    virtual std::unique_ptr<ParticleIterator> end() = 0;

    virtual std::unique_ptr<PairParticleIterator> beginPairParticle() = 0;
    virtual std::unique_ptr<PairParticleIterator> endPairParticle() = 0;
};
