#pragma once
#include <particle/Particle.h>
#include <particle/container/ParticleContainer.h>
#include <particle/iterator/particleIterator/ParticleIteratorLinkedCell.h>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>
#include <vector>

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate
 * particles or particle pairs
 */
class ParticleContainerLinkedCell : ParticleContainer {

public:

        /**
         * @brief wraps a vector in a ParticleContainer
         * @param particlesArg the particle vector to be wrapped
         * @return the object of the ParticleContainer
         */
        ParticleContainerLinkedCell(std::array<double, 3> domainDimsArg, double cutoffRadiusArg);

        void addParticle(const Particle &particle) override;
        int size() override;
        double getAverageVelocity() override;
        void setAverageVelocity(double averageVelocityArg) override;
        Particle &getParticle(int index) override;
        /**
         * @brief provides an iterator for iterating through all particles when using the linked cell algorithm
         * @return the particle iterator
         */
        ParticleIteratorLinkedCell begin();
        /**
         * @brief provides an iterator pointing to the end of the particles when using the linked cell algorithm
         * @return the end of the particle iterator
         */
        ParticleIteratorLinkedCell end();
        /**
         * @brief provides an iterator for iterating through pairs of particles when using the linked cell algorithm
         * @return the pair particle iterator
         */
        PairParticleIteratorLinkedCell beginPairs();
        /**
         * @brief provides an iterator pointing to the end of the pairs when using the linked cell algorithm
         * @return the end of the pair particle iterator
         */
        PairParticleIteratorLinkedCell endPairs();

        /**
         * @brief provides an iterator for iterating through pairs of boundary particles and their ghosts. The first
         * element in a pair is the actual particle whereas the second element is the ghost
         * @return the boundary particle and ghost pair iterator
         */
        PairParticleIteratorBoundaryNHalo beginGhosts();
        /**
         * @brief provides an iterator pointing to the end of the boundary and ghost pairs
         * @return the end of the boundary and ghost pair iterator
         */
        PairParticleIteratorBoundaryNHalo endGhosts();
        /**
         * @brief initializes the mesh which is a grid of cells
         * @param domainSize the 3d size of the domain
         * @param cutoffRadius the cutoffRadius (used for determining for cell size)
         */
        void initMesh(std::array<double, 3> domainSize, double cutoffRadius);
        /**
         * @brief converts the discrete (index) coordinates of a cell in the mesh to
         * the index at which the cell is in the mesh vector
         * @param coord
         * @return
         */
        int discreteCoordsToIndex(std::array<int, 3> coord);
        /**
         * @brief converts the continuous coordinates (which can be anywhere in the
         * mesh) to the index of the cell in the mesh vector where these coordinates
         * (e.g. of a vector) lie in
         * @param coord
         * @return
         */
        int continuousCoordsToIndex(std::array<double, 3> coord);
        /**
         * @brief corrects the indecies of all particles in the linked cell container mesh based on their position
         */
        void correctAllParticleIndecies();
        std::vector<Cell>& getMesh();
        Cell& getCell(int idx);
        std::array<size_t, 3> getNumCells();
    private:
        /**
         * @brief mesh contains all grid cells
         */
        std::vector<Cell> mesh;
        /**
         * @brief the average brownian velocity of the particles
         */
        double averageVelocity;
        std::vector<double> domainDims;      // size of the domain of this particleContainer
        double cutoffRadius; // = cutoffRadius
        std::array<double, 3> cellSize; // dimensions of one cell
        std::array<size_t, 3> numCells; // number of cells per dimensions
        /**
         * @brief corrects the index of a particle in the linked cell container mesh based on it's position
         * @param p the particle to correct the position of
         */
        void correctParticleIndex(Particle& p);
};
