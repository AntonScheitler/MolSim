#pragma once
#include "particle/iterator/particleIterator/ParticleIterator.h"
#include <particle/Particle.h>
#include <particle/cell/Cell.h>
#include <vector>


/**
 * @brief an iterator which enables iteration over each particle separately
 */
class ParticleIteratorLinkedCell : public ParticleIterator{

    public:
        /**
         * @brief create an instance of a ParticleIterator for the Linked Cell Particle Container
         * @param it the vector iterator to build the ParticleIterator upon
         * @param end the end of the vector iterator
         * @return an instance of a ParticleIterator
         */
        ParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator endArg);

        /**
         * @brief Dereference current Particle in this Linked Cell ParticleContainer
         * @return current Particle
         */
        reference operator*() override;

        /**
         * @brief Increment current Particle in this Linked Cell ParticleContainer
         * @return this ParticleContainer object
         */
        ParticleIteratorLinkedCell &operator++() override;

        /**
         * @brief Check whether the current Particle of this ParticleContainer is not equal to the current
         * Particle of another ParticleContainer.
         * @param other Other ParticleContainer
         * @return True if the current particles are not equal
         */
        bool operator!=(const ParticleIterator& other) override;

    private:
        /**
         * @brief an iterator pointing to the current cell
         */
        std::vector<Cell>::iterator currentCell;
        /**
         * @brief an iterator pointing to the end of the mesh
         */
        std::vector<Cell>::iterator end;
        /**
         * @brief an iterator pointing to the current particle within the current cell
         */
        std::vector<Particle>::iterator currentParticle;
};
