#include "Particle.h"
#include <iterator>
#include <vector>

#pragma once

/**
 * @brief an iterator which enables iteration over each particle separately
 */
class ParticleIterator {

public:
    using iterator_category = std::forward_iterator_tag;
    using reference = Particle &;

    /**
     * @brief create an instance of a ParticleIterator
     * @param it the vector iterator to build the ParticleIterator upon
     * @return an instance of a ParticleIterator
     */
    ParticleIterator(std::vector<Particle>::iterator it);

    /**
     * @brief Dereference current Particle in this ParticleContainer
     * @return current Particle
     */
    reference operator*();

    /**
     * @brief Increment current Particle in this ParticleContainer
     * @return this ParticleContainer object
     */
    ParticleIterator &operator++();

    /**
     * @brief Check whether the current Particle of this ParticleContainer is not equal to the current
     * Particle of another ParticleContainer.
     * @param other Other ParticleContainer
     * @return True if the current particles are not equal
     */
    bool operator!=(const ParticleIterator &other);

private:
    std::vector<Particle>::iterator current;
};


