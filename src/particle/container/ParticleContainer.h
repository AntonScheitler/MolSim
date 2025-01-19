#include "../Particle.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIteratorNonFixedParticles.h"
#include <memory>

#pragma once

/**
 * @brief base class for different types of particle containers
 */
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

    /**
     * @brief copies this particleContainer
     * @return
     */
    virtual std::unique_ptr<ParticleContainer> copy() = 0;

    // returns iterators
    virtual ParticleIterator begin() = 0;

    virtual ParticleIterator end() = 0;

    virtual std::unique_ptr<PairParticleIterator> beginPairParticle() = 0;

    virtual std::unique_ptr<PairParticleIterator> endPairParticle() = 0;

    /**
     * @brief provides an iterator for iterating through particles that are not fixed
     * @return the non-fixed-particle iterator
     */
    virtual ParticleIteratorNonFixedParticles beginNonFixedParticles() = 0;
    /**
     * @brief provides an iterator pointing to the end of the particles that are not fixed
     * @return the end of the non-fixed-particle iterator
     */
    virtual ParticleIteratorNonFixedParticles endNonFixedParticles() = 0;
};
