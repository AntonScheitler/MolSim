#pragma once

#include <particle/Particle.h>
#include <particle/container/ParticleContainer.h>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>
#include <vector>

/**
 * @brief manages a set of particles using the linked cell algorithm
 */
class ParticleContainerLinkedCell : public ParticleContainer {

public:

    /**
     * @brief creates an instance of a particle container for the linked cell algorithm
     * @param domainSizeArg the size of the domain
     * @param cutoffRadiusArg radius past which forces between particles don't need to be computed
     * @param boundaryConfigArg the configuration of the boundaries of this container (defaults to outflow everywhere)
     * @return an instance of the particle container for the linked cell algorithm
     */
    ParticleContainerLinkedCell(std::array<double, 3> domainSizeArg, double cutoffRadiusArg,
                                struct boundaryConfig boundaryConfigArg = {{outflow, outflow},
                                                                           {outflow, outflow},
                                                                           {outflow, outflow}});

    void addParticle(const Particle &particle) override;

    int size() override;

    std::unique_ptr<ParticleContainer> copy() override;

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
     * @brief determinesthe distance between two points across periodic boundaries. The distance is defined via a point and a
     * vector. The periodic distance must be across two boundaries which are marked as periodic
     * @param coord a point that the periodic distance passes through
     * @param v a vector to base the periodic distance off of
     * @return the periodic distance vector that is based off of v and passes through coord. if there is no such vector,
     * for example if it is across non-periodic boundaries, then an array of DOUBLE_MAX_VALUE is returned
     */
    std::array<double, 3> getPeriodicDistanceVector(const std::array<double, 3>& point1, const std::array<double, 3>& point2, std::array<double, 3>& v);

    /**
     * @brief applies periodic boundaries to the specified array
     * @param coord the coordinate array to apply the periodic boundaries to 
     */
    std::array<double, 3> applyPeriodicBoundaries(std::array<double, 3> coord);

    /**
     * @brief corrects the indices of all particles in the linked cell container mesh based on their position
     */
    void correctCellMembershipAllParticles();

    /**
     * @brief computes the neighborCellsMatrix 
     */
    void computeNeighborCellsMatrix();

    std::vector<Cell> &getMesh();

    Cell &getCell(int idx);

    std::array<size_t, 3> getNumCells();

    std::array<double, 3> getCellSize();

    double getCutoffRadius();

private:
    /**
     * @brief mesh contains all grid cells
     */
    std::vector<Cell> mesh;;

    /**
     * @brief the particles of this container
     */
    std::vector<Particle> particles;

    /**
     * @brief a vector of vectors containing indices for the neighborCells of the cell at the given index 
     */
    std::vector<std::vector<size_t>> neighborCellsMatrix;
    /**
     * @brief the size of the domain for the container
     */
    std::array<double, 3> domainSize;
    /**
     * @brief the cutoffRadius past which no computations between particle pairs should take place.
     * the mesh is always divided so that the cutoffradius for a particle extends at most to the particles in the
     * neighboring cells
     */
    double cutoffRadius;
    std::array<double, 3> cellSize; // dimensions of one cell
    std::array<size_t, 3> numCells; // number of cells per dimensions

    /**
     * @brief the boundary configuration for this linked cell container
     */
    struct boundaryConfig boundaryConfig;

    /**
     * @brief corrects the index of a particle in the linked cell container mesh based on it's position
     * @param p the particle to correct the position of
     * @returns true if the particle should be removed from its old cell
     */
    bool correctCellMembershipSingleParticle(size_t particleIdx);
};
