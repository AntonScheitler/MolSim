#pragma once

#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <particle/Particle.h>
#include <vector>
#include <particle/cell/Cell.h>
#include <particle/boundary/Boundary.h>

/**
 * @brief an iterator which enables iteration over pairs of boundary particles and their ghost cells. The second particle in any
 * pair is a ghost particle. So in a pair <a, b>, a would be a real particle and b the ghost
 */
class PairParticleIteratorBoundaryNHalo : public PairParticleIterator {
public:
    /**
     * @brief create an instance of a PairParticleIterator for boundary particles and their ghost particles
     * @param currentCellArg an iterator pointing to the start of the mesh
     * @param currentCellEndArg an iterator pointing to the end of the mesh
     * @param meshArg the mesh of which the boundary will be iterated through
     * @param numCellsArg the number of cells per dimension of the mesh
     * @param cellSizeArg the size of a cell
     * @param boundaryConfigArg the boundary configuration of the container this iterator iterates through
     * @return an instance of a PairParticleIterator for boundary particles and their ghost particles
     */
    PairParticleIteratorBoundaryNHalo(std::vector<Cell>::iterator currentCellArg,
                                      std::vector<Cell>::iterator currentCellEndArg, std::vector<Cell>& meshArg,
                                      std::array<size_t, 3> numCellsArg,
                                      std::array<double, 3> cellSizeArg, struct boundaryConfig boundaryConfigArg,
                                      std::vector<Particle>& particlesArg);

    /**
     * @brief dereferences this PairParticleIterator and gets the current particle/ghost pair.
     * @return a pair of real and ghost particles
     */
    reference operator*() override;

    /**
     * @brief increment this iterator, i.e. get the next distinct pair of particles
     * @return the incremented iterator
     */
    PairParticleIteratorBoundaryNHalo &operator++() override;

    /**
     * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator. This operator is
     * kept very simple and might label iterators as equal, even if they are really not. This is because it is
     * sufficient to distinguish an iterator at the beginning from one at end of the iteration process
     * @param other the other PairParticleIterator
     * @return whether or not the iterators are "equal"
     */
    bool operator!=(const PairParticleIterator &other) override;

    /**
     * @brief returns the ghost particles for the currentParticle
     * @return the vector of ghosts
     */
    void updateGhostsVector();

private:
    /**
     * @brief the mesh to iterate through
     */
    std::vector<Cell>& mesh;

    std::vector<Particle>& particles;
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
    std::vector<size_t>::iterator currentParticleIdx;
    /**
     * @brief a vector of the ghost particles of the current particle
     */
    std::vector<std::unique_ptr<Particle>> ghostsVector;
    /**
     * @brief an iterator pointing to the ghost particles for the currentParticle
     */
    std::vector<std::unique_ptr<Particle>>::iterator currentGhost;

    /**
     * @brief an iterator pointing to the end of the ghost particles for the currentParticle
     */
    std::vector<std::unique_ptr<Particle>>::iterator currentGhostEnd;
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

    /**
     * @brief increments currentCell until it reaches a cell, which is a boundary and non-empty
     * @param stepToNext if false, currentCell will not be updated if it already points to a non-empty boundary cell
     */
    void stepToNonEmptyBoundaryCell(bool stepToNext);
};
