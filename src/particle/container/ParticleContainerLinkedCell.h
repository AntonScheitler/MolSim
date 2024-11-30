#pragma once
#include <particle/Particle.h>
#include <particle/container/ParticleContainer.h>
#include <particle/iterator/particleIterator/ParticleIteratorLinkedCell.h>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>
#include <vector>

/**
 * @brief manages a set of particles using the linked cell algorithm
 */
class ParticleContainerLinkedCell : ParticleContainer {

public:

        /**
         * @brief creates an instance of a particle container for the linked cell algorithm 
         * @param domainSizeArg the size of the domain
         * @param cutoffRadiusArg radius past which forces between particles don't need to be computed
         * @return an instance of the particle container for the linked cell algorithm
         */
        ParticleContainerLinkedCell(std::array<double, 3> domainSizeArg, double cutoffRadiusArg);

        void addParticle(const Particle &particle) override;
        int size() override;
        std::unique_ptr<ParticleIterator> begin() override;
        std::unique_ptr<ParticleIterator> end() override;
        std::unique_ptr<PairParticleIterator> beginPairParticle() override;
        std::unique_ptr<PairParticleIterator> endPairParticle() override;

        /**
         * @brief provides an iterator for iterating through pairs of boundary particles and their ghosts. The first
         * element in a pair is the actual particle whereas the second element is the ghost
         * @return the boundary particle and ghost pair iterator
         */
        PairParticleIteratorBoundaryNHalo beginPairGhost();
        /**
         * @brief provides an iterator pointing to the end of the boundary and ghost pairs
         * @return the end of the boundary and ghost pair iterator
         */
        PairParticleIteratorBoundaryNHalo endPairGhost();
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
        std::array<double, 3> getCellSize();
    private:
        /**
         * @brief mesh contains all grid cells
         */
        std::vector<Cell> mesh;;
        std::array<double, 3> domainSize;      // size of the domain of this particleContainer
        double cutoffRadius; // = cutoffRadius
        std::array<double, 3> cellSize; // dimensions of one cell
        std::array<size_t, 3> numCells; // number of cells per dimensions
        /**
         * @brief corrects the index of a particle in the linked cell container mesh based on it's position
         * @param p the particle to correct the position of
         */
        void correctParticleIndex(Particle& p);
};

