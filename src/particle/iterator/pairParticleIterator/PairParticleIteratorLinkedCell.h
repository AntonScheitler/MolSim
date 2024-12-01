#pragma once
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <cstddef>
#include <particle/Particle.h>
#include <unordered_set>
#include <vector>
#include <particle/cell/Cell.h>


/**
 * @brief an iterator which enables iteration over pairs of particles. The elements of those pairs are always
 * distinct ordering is not considered. This means, that a specific pair <a, b> will only appear once when
 * iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIteratorLinkedCell: public PairParticleIterator {
    public:
        /**
         * @brief create an instance of a PairParticleIterator for the Linked Cell algorithm
         * @param particleContainer the particleContainer to form the pairs for
         * @return an instance of the Linked Cell PairParticleIterator
         */
        PairParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator end, std::vector<Cell>& meshArg, std::array<size_t, 3> numCellsArg);
        /**
         * @brief Dereference this PairParticleIterator, i.e. get the current pair of Particles
         * @return std::pair of Particles
         */
        reference operator*() override;
        /**
         * @brief Increment this iterator, i.e. get the next distinct pair of particles
         * @return this PairParticleIterator updated
         */
        PairParticleIteratorLinkedCell& operator++() override;
        /**
         * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator
         * @param other other PairParticleIterator
         * @return True if iterators not equal
         */
        bool operator!=(const PairParticleIterator &other) override;
        /**
         * @brief determines the vector of neighbors for the current cell
         * @return the vector of neighbors for the current cell
         */
        std::vector<Cell> getNeighborCells();

        void incrementCurrCellIdx();

    private:
        /**
         * @brief the mesh to iterate through 
         */
        std::vector<Cell> mesh;
        /**
         * @brief the mesh index of the current cell
         */
        std::array<int, 3> currentCellIdx;
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
        std::vector<Particle>::iterator currentParticle;
        /**
         * @brief a vector of the neighbor cells of the current cell
         */
        std::vector<Cell> neighborCellsVector;
        /**
         * @brief an iterator pointing to the neighbor of the current cell
         */
        std::vector<Cell>::iterator neighborCell;
        /**
         * @brief an iterator pointing to the end of the neighbor cells
         */
        std::vector<Cell>::iterator neighborEnd;
        /**
         * @brief an iterator pointing to the current particle within the partner cell to create pairs with
         */
        std::vector<Particle>::iterator neighborParticle;
        /**
         * @brief the number cells per dimension
         */
        std::array<size_t, 3> numCells;
        /**
         * @brief struct for hashing particles. this is necessary because particles that have been iterated through are
         * stored in a hashset
         */
        struct ParticleHash {
            size_t operator()(const Particle& p) const {
                return (std::hash<double>()(p.getX()[0]) << 6) ^
                    (std::hash<double>()(p.getX()[1]) << 4) ^
                    (std::hash<double>()(p.getX()[2]) << 2);
            }
        };
        /**
         * @brief struct for comparing particles. this is necessary because particles that have been iterated through are
         * stored in a hashset
         */
        struct ParticleEqual {
            bool operator()(const Particle& p1, const Particle& p2) const {
                return p1 == p2;
            }
        };
        /**
         * @brief a set containing all particles for which all paris have been iterated through
         */
        std::unordered_set<Particle, ParticleHash, ParticleEqual> completedPairs;

        // TODO
        void stepToNextPair();

        /**
         * @brief skips cells until currentCell reaches the next non-empty cell. If the currentCell is non-empty no step is executed
         */
        void currentStepToViableCell(bool stepBefore);

        /**
         * @brief skips particles within currentCell until a particle is reached, with which pairs can be formed.
         * If currentParticle already points to such a particle, no steps are executed
         */
        void currentStepToViableParticle();

        /**
         * @brief skips cells and particles until neighborCell reaches the next non-empty cell which contains particles that can form
         * a pair. neigborParticle is then set to such a particle
         * If those conditions are already satisfied, no steps are executed
         */
        void neighborStepToViableCell(bool stepBefore);

        /**
         * @brief skips particles within neighborCell until a particle is reached, with which a pair can be formed.
         * If neighborParticle already points to such a particle, no steps are executed
         */
        void neighborStepToViableParticle();
};
