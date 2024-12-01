#pragma once
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <cstddef>
#include <particle/Particle.h>
#include <vector>
#include <particle/cell/Cell.h>
#include <particle/boundary/Boundary.h>

/**
 * @brief an iterator which enables iteration over pairs of particles. The elements of those pairs are always
 * distinct ordering is not considered. This means, that a specific pair <a, b> will only appear once when
 * iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIteratorBoundaryNHalo: public PairParticleIterator {
    public:
        /**
         * @brief create an instance of a PairParticleIterator for iterating through the boundary cells of the mesh and
         * return pairs of boundary cells and their ghost particles in the halo cells
         * @param meshArg the mesh of which the boundary will be iterated through
         * @param numCellsArg the number of cells per dimension of the mesh
         * @param cellSizeArg the size of a cell
         * @param boundaryConfigArg the boundary configuration of the container this iterator iterates through
         * @return an instance of a PairParticleIterator for boundary and halo cells 
         */
        PairParticleIteratorBoundaryNHalo(std::vector<Cell>::iterator currentCellArg, std::vector<Cell>::iterator currentCellEndArg, std::vector<Cell> meshArg, std::array<size_t, 3> numCellsArg, 
                std::array<double, 3> cellSizeArg, struct boundaryConfig boundaryConfigArg);
        /**
         * @brief dereferences this PairParticleIterator and gets the current particle/ghost pair. The second particle in any
         * pair is a ghost particle. So in a pair <a, b>, a would be a real particle and b the ghost
         * @return a pair of real and ghost particles
         */
        reference operator*() override;
        /**
         * @brief Increment this iterator, i.e. get the next distinct pair of particles
         * @return this PairParticleIterator updated
         */
        PairParticleIteratorBoundaryNHalo& operator++() override;
        /**
         * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator
         * @param other other PairParticleIterator
         * @return True if iterators not equal
         */
        bool operator!=(const PairParticleIterator &other) override;
        /**
         * @brief returns the ghost particles for the currentParticle 
         * @return the vector of ghosts
         */
        std::vector<Particle> createGhostParticles();
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
         * @brief an iterator pointing to the end of the mesh
         */
        std::vector<Cell>::iterator currentCellEnd;
        /**
         * @brief an iterator pointing to the current particle within the current cell
         */
        std::vector<Particle>::iterator currentParticle;
        /**
         * @brief a vector of the ghost particles of the current particle
         */
        std::vector<Particle> ghostsVector;
        /**
         * @brief an iterator pointing to the ghost particles for the currentParticle
         */
        std::vector<Particle>::iterator currentGhost;

        /**
         * @brief an iterator pointing to the end of the ghost particles for the currentParticle
         */
        std::vector<Particle>::iterator currentGhostEnd;
        /**
         * @brief the number of cells per dimension
         */
        std::array<size_t, 3> numCells;
        /**
         * @brief the size of a cell in the mesh
         */
        std::array<double, 3> cellSize;
        /**
         * @brief the boundary configuration for the container this iterator runs on
         */
        struct boundaryConfig boundaryConfig;
        /**
         * @brief increments the currentCellIdx
         */
        void incrementCurrentCellIdx();
        void stepToNonEmptyBoundaryCell(bool stepToNext);
};
