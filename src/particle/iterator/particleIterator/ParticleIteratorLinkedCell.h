#include <particle/Particle.h>
#include <particle/cell/Cell.h>
#include <vector>

#pragma once

/**
 * @brief an iterator which enables iteration over each particle separately
 */
class ParticleIteratorLinkedCell {

    public:
        using iterator_category = std::forward_iterator_tag;
        using reference = Particle &;
        /**
         * @brief create an instance of a ParticleIterator for the Linked Cell Particle Container
         * @param it the vector iterator to build the ParticleIterator upon
         * @return an instance of a ParticleIterator
         */
        ParticleIteratorLinkedCell(std::vector<Cell>::iterator it);

        /**
         * @brief Dereference current Particle in this Linked Cell ParticleContainer
         * @return current Particle
         */
        reference operator*();

        /**
         * @brief Increment current Particle in this Linked Cell ParticleContainer
         * @return this ParticleContainer object
         */
        ParticleIteratorLinkedCell &operator++();

        /**
         * @brief Check whether the current Particle of this ParticleContainer is not equal to the current
         * Particle of another ParticleContainer.
         * @param other Other ParticleContainer
         * @return True if the current particles are not equal
         */
        bool operator!=(const ParticleIteratorLinkedCell &other);

    private:
        /**
         * @brief an iterator pointing to the current cell
         */
        std::vector<Cell>::iterator currentCell;
        /**
         * @brief an iterator pointing to the current particle within the current cell
         */
        std::vector<Particle>::iterator currentParticle;
};
