#include <particle/Particle.h>
#include <particle/iterator/particleIterator/ParticleIterator.h>
#include <vector>

#pragma once

/**
 * @brief an iterator which enables iteration over each particle separately
 */
class ParticleIteratorDirectSum : public ParticleIterator {

public:
    /**
     * @brief create an instance of a ParticleIterator
     * @param it the vector iterator to build the ParticleIterator upon
     * @return an instance of a ParticleIterator
     */
    ParticleIteratorDirectSum(std::vector<Particle>::iterator it);

    /**
     * @brief Dereference current Particle in this ParticleContainer
     * @return current Particle
     */
    reference operator*() override;

    /**
     * @brief Increment current Particle in this ParticleContainer
     * @return this ParticleContainer object
     */
    ParticleIteratorDirectSum &operator++() override;

    /**
     * @brief Check whether the current Particle of this ParticleContainer is not equal to the current
     * Particle of another ParticleContainer.
     * @param other Other ParticleContainer
     * @return True if the current particles are not equal
     */
    bool operator!=(const ParticleIterator &other) override;

private:
    /**
     * @brief an iterator pointing to the current particle
     */
    std::vector<Particle>::iterator current;
};
