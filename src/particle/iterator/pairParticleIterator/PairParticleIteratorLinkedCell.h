#pragma once

#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <cmath>
#include <cstddef>
#include <particle/Particle.h>
#include <unordered_set>
#include <vector>
#include <particle/cell/Cell.h>


/**
 * @brief an iterator which enables iteration over pairs of particles when using the linked cell algorithm.
 * Any specific pair <a, b> will only appear once when iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIteratorLinkedCell : public PairParticleIterator {
    public:
        /**
         * @brief create an instance of a PairParticleIterator for the Linked Cell algorithm
         * @param it an iterator pointing to the start of the mesh
         * @param end an iterator pointing to the end of the mesh
         * @param meshArg the mesh to iterate through
         * @param numCellsArg the number of cells per dimension of the mesh
         * @return an instance of the Linked Cell PairParticleIterator
         */
        PairParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator end,
                std::vector<Cell> &meshArg, std::vector<Particle>& particlesArg, std::vector<std::vector<size_t>>& neighborCellsMatrix);

        /**
         * @brief Dereference this PairParticleIterator, i.e. get the current pair of Particles
         * @return a pair of particles
         */
        reference operator*() override;

        /**
         * @brief increments the iterator to get the next distinct pair of particles
         * @return the incremented iterator
         */
        PairParticleIteratorLinkedCell &operator++() override;
        /**
         * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator. The
         * false-positive rate of this comparator is quite large since it is sufficient to distinguish it from the end
         * @param other the other PairParticleIterator
         * @return whether or not the provided iterator is different from this one
         */
        bool operator!=(const PairParticleIterator &other) override;
    private:
        /**
         * @brief the mesh to iterate through 
         */
        std::vector<Cell>& mesh;

        /**
         * @brief the particles to iterate through 
         */
        std::vector<Particle>& particles;
        /**
         * @brief the mesh index of the current cell
         */
        size_t currentCellIdx;
        /**
         * @brief an iterator pointing to the current cell
         */
        std::vector<Cell>::iterator currentCell;
        /**
         * @brief an iterator pointing to the end of the cells
         */
        std::vector<Cell>::iterator end;
        /**
         * @brief an iterator pointing to the current particle within the current cell
         */
        std::vector<size_t>::iterator currentParticleIdx;
        /**
         * @brief a vector of vectors containing indices for the neighborCells of the cell at the given index 
         */
        std::vector<std::vector<size_t>>& neighborCellsMatrix;
        /**
         * @brief a vector of the neighbor cells of the current cell
         */
        std::vector<size_t> neighborCellsVector;
        /**
         * @brief an iterator pointing to the neighbor of the current cell
         */
        std::vector<size_t>::iterator neighborCell;
        /**
         * @brief an iterator pointing to the end of the neighbor cells
         */
        std::vector<size_t>::iterator neighborEnd;
        /**
         * @brief an iterator pointing to the current particle within the partner cell to create pairs with
         */
        std::vector<size_t>::iterator neighborParticleIdx;

        /**
         * @brief struct for hashing particles. this is necessary because particles that have been iterated through are
         * stored in a hashset
         */
        struct ParticleHash {
            size_t operator()(const Particle &p) const {
                return p.getId();
            }
        };

        /**
         * @brief struct for comparing particles. this is necessary because particles that have been iterated through are
         * stored in a hashset
         */
        struct ParticleEqual {
            bool operator()(const Particle &p1, const Particle &p2) const {
                return p1 == p2;
            }
        };

        /**
         * @brief a set containing all particles for which all pairs have been iterated through
         */
        std::unordered_set<size_t> completedParticles;

        /**
         * @brief skips cells until currentCell reaches the next non-empty cell for which there are still pairs to be computed
         * @param stepBefore if false, the currentCell will not change if it already points to a cell for which there
         * are still pairs to be computed
         */
        void currentStepToViableCell(bool stepBefore);

        /**
         * @brief skips particles within the currentCell until a particle is reached, for which there are pairs which need to be computed.
         * If currentParticle already points to such a particle, no steps are executed
         */
        void currentStepToViableParticle();

        /**
         * @brief skips neighbor cells until a cell is reached with which pairs can be formed
         * @param stepBefore if false, the neighborCell will not change if it already points to a cell with which pairs can be formed
         */
        void neighborStepToViableCell(bool stepBefore);

        /**
         * @brief skips particles within neighborCell until a particle is reached, with which a pair can be formed.
         * If neighborParticle already points to such a particle, no steps are executed
         */
        void neighborStepToViableParticle();
};
