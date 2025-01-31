#pragma once

#include <particle/Particle.h>
#include <vector>


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
     * @param endArg an iterator that points to the end of the particle vector
     * @return an instance of a ParticleIterator
     */
    ParticleIterator(std::vector<Particle>::iterator it, std::vector<Particle>::iterator endArg);

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
    /**
     * @brief an iterator pointing to the current particle
     */
    std::vector<Particle>::iterator current;

    /**
     * @brief an iterator pointing to the end of the particle vector
     */
    std::vector<Particle>::iterator end;
};
