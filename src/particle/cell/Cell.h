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
     * @brief adds a particle to the cell
     * @param particle the particle to add
     */
    void addParticle(const Particle &particle);

    /**
     * @brief removes the specified particle from the cell
     * @param particle the particle to remove
     */
    void removeParticle(size_t particleId);

    /**
     * @brief returns the particles that are stored in this cell
     * @return the particles as a vector
     */
    std::vector<Particle> &getParticles();

    /**
     * returns the number of particles that are stored in this cell
     * @return number number of particles
     */
    int size();

    /**
     * used for comparing this cell to another cell for equality
     * @param other the other cell
     * @return
     */
    bool operator==(const Cell &other);

    /**
     * @brief the flag which specifies whether this cell is a boundary cell
     */
    bool isBoundary;

private:
    /**
     * @brief the vector which stores all particles of this cell
     */
    std::vector<Particle> particles;
    /**
     * @brief the id of this cell (used for comparing cells)
     */
    size_t cellId;
};
