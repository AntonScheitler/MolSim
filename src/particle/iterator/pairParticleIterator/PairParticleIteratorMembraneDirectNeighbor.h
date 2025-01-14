#pragma once

#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <cmath>
#include <particle/Particle.h>
#include <vector>
#include <particle/cell/Cell.h>

/**
 * @brief an iterator which enables iteration over pairs of direct neighbors in a membrane
 * Any specific pair <a, b> will only appear once when iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIteratorMembraneDirectNeighbor : public PairParticleIterator {
    public:
        /**
         * @brief create an instance of a PairParticleIterator for the direct neighbors in a membrane
         * @param it an iterator pointing to the start of the particles
         * @param end an iterator pointing to the end of the particles
         * @param particlesArg the particles vector to iterate through
         * @return an instance of the Direct Neighbor Membrane PairParticleIterator
         */
        PairParticleIteratorMembraneDirectNeighbor(std::vector<Particle>::iterator it, std::vector<Particle>::iterator end,
                std::vector<Particle>& particlesArg);

        /**
         * @brief Dereference this PairParticleIterator, i.e. get the current pair of Particles
         * @return a pair of particles
         */
        reference operator*() override;

        /**
         * @brief increments the iterator to get the next distinct pair of particles
         * @return the incremented iterator
         */
        PairParticleIteratorMembraneDirectNeighbor &operator++() override;

        /**
         * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator
         * @param other the other PairParticleIterator
         * @return whether or not the provided iterator is different from this one
         */
        bool operator!=(const PairParticleIterator &other) override;
    private:
        /**
         * @brief the particles to iterate through 
         */
        std::vector<Particle>& particles;
        /**
         * @brief an iterator pointing to the current particle in the iteration
         */
        std::vector<Particle>::iterator curr;
        /**
         * @brief the current relative position of the neighbor from the current particle in the iteration.
         * currNeighborPos always switches back and forth between right and top
         */
        DirectNeighborPos currNeighborPos;
        /**
         * @brief an iterator pointing to the end of the particles
         */
        std::vector<Particle>::iterator end;

        /**
         * @brief steps to the next particle which has at least a right or top neighbor
         */
        void stepToNextViableParticle();
};
