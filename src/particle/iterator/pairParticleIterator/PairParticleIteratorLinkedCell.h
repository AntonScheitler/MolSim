#pragma once
#include <cstddef>
#include <particle/Particle.h>
#include <iterator>
#include <unordered_set>
#include <vector>
#include <particle/cell/Cell.h>


/**
 * @brief an iterator which enables iteration over pairs of particles. The elements of those pairs are always
 * distinct ordering is not considered. This means, that a specific pair <a, b> will only appear once when
 * iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIteratorLinkedCell {

    public:
        using iterator_category = std::forward_iterator_tag;
        using reference = std::pair<Particle &, Particle &>;

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
        reference operator*();

        /**
         * @brief Increment this iterator, i.e. get the next distinct pair of particles
         * @return this PairParticleIterator updated
         */
        PairParticleIteratorLinkedCell& operator++();

        /**
         * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator
         * @param other other PairParticleIterator
         * @return True if iterators not equal
         */
        bool operator!=(const PairParticleIteratorLinkedCell &other);

        /**
         * @brief determines the vector of neighbors for the current cell
         * @return the vector of neighbors for the current cell
         */
        std::vector<Cell> getNeighborCells();

    private:
        /**
         * @brief the mesh to iterate through 
         */
        std::vector<Cell> mesh;
        /**
         * @brief the mesh index of the current cell
         */
        int currentCellIdx;
        /**
        * @brief an iterator pointing to the current cell
         */
        std::vector<Cell>::iterator currentCell;
        /**
        * @brief an iterator pointing to the current particle within the current cell
         */
        std::vector<Particle>::iterator currentParticle;
        /**
        * @brief a vector of the neighbor cells of the current cell
         */
        std::vector<Cell> neighborCellsVector;
        /**
        * @brief an iterator pointing to the the neighbor of the current cell
         */
        std::vector<Cell>::iterator neighborCell;
        /**
        * @brief an iterator pointing to the end of the neighborCell iterator
         */
        std::vector<Cell>::iterator neighborCellEnd;
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
            size_t operator()(const Particle& p) const;
        };
        /**
         * @brief struct for comparing particles. this is necessary because particles that have been iterated through are
         * stored in a hashset
         */
        struct ParticleEqual {
            bool operator()(const Particle& p1, const Particle& p2) const;
        };
        /**
         * @brief a set containing all particles for which all paris have been iterated through
         */
        std::unordered_set<Particle, ParticleHash, ParticleEqual> completedPairs;
};
