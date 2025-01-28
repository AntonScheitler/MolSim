#pragma once

#include "vector"
#include <particle/Particle.h>

/**
 * @brief class for representing a single cell of the mesh of the ParticleContainerLinkedCell
 */
class Cell {

public:
    /**
     * @brief create a cell
     * @param isBoundaryCell specify whether this cell is a boundaryCell
     * @param cellIdArg the id of this cell
     */
    Cell(bool isBoundaryCell, size_t cellIdArg);

    /**
     * @brief makes a particle a member of this cell by adding it's index to particleIndices
     * @param idx the index of the particle to be added
     */
    void addParticleIdx(const size_t idx);

    /**
     * @brief removes the specified particle from the cell
     * @param idx the index of the particle to remove
     */
    void removeParticle(const size_t idx);

    /**
     * @brief returns the indices of particles that are stored in this cell
     * @return the particle indices as a vector
     */
    std::vector<size_t> &getParticlesIndices();

    /**
     * @brief returns the number of particles that are stored in this cell
     * @return number number of particles
     */
    int size();

    /**
     * @brief used for comparing this cell to another cell for equality
     * @param other the other cell
     * @return
     */
    bool operator==(const Cell &other);

    /**
     * @brief returns the id of this cell
     * @return the id of this cell
     */
    size_t getId();

    /**
     * @brief the flag which specifies whether this cell is a boundary cell
     */
    bool isBoundary;


private:
    /**
     * @brief the vector which stores the indices of the particles that are members of this cell
     */
    std::vector<size_t> particleIndices;
    /**
     * @brief the id of this cell (used for comparing cells)
     */
    size_t cellId;
};
