#include <particle/Particle.h>

#pragma once

/**
 * @brief an iterator which enables iteration over each particle separately
 */
class ParticleIterator {

public:
    using iterator_category = std::forward_iterator_tag;
    using reference = Particle &;

    ParticleIterator() = default;

    virtual ~ParticleIterator() = default;

    /**
     * @brief Dereference current Particle in this ParticleContainer
     * @return current Particle
     */
    virtual reference operator*() = 0;

    /**
     * @brief Increment current Particle in this ParticleContainer
     * @return this ParticleContainer object
     */
    virtual ParticleIterator &operator++() = 0;

    /**
     * @brief Check whether the current Particle of this ParticleContainer is not equal to the current
     * Particle of another ParticleContainer.
     * @param other Other ParticleContainer
     * @return True if the current particles are not equal
     */
    virtual bool operator!=(const ParticleIterator &other) = 0;
};
