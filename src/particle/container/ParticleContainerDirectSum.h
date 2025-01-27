#include <memory>
#include <vector>
#include "../Particle.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIteratorNonFixedParticles.h"
#include <particle/iterator/pairParticleIterator/PairParticleIteratorDirectSum.h>
#include <particle/container/ParticleContainer.h>

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
    explicit ParticleContainerDirectSum(const std::vector<Particle> &particlesArg);

    void addParticle(const Particle &particle) override;

    int size() override;

    std::unique_ptr<ParticleContainer> copy() override;

    Particle &getParticle(size_t index) override;

    ParticleIterator begin() override;

    ParticleIterator end() override;

    /**
     * @brief provides an iterator for iterating through pairs of particles
     * @return the pair particle iterator
     */
    std::unique_ptr<PairParticleIterator> beginPairParticle() override;

    /**
     * @brief provides an iterator pointing to the end of the pairs
     * @return the end of the pair particle iterator
     */
    std::unique_ptr<PairParticleIterator> endPairParticle() override;

    ParticleIteratorNonFixedParticles beginNonFixedParticles() override;

    ParticleIteratorNonFixedParticles endNonFixedParticles() override;

protected:
    /**
     * @brief the particles wrapped by this container
     */
    std::vector<Particle> particles;
};
