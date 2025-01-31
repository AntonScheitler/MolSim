#pragma once

#include "particle/iterator/pairParticleIterator/PairParticleIteratorMembraneDiagonalNeighbor.h"
#include "particle/iterator/pairParticleIterator/PairParticleIteratorMembraneDirectNeighbor.h"
#include "particle/iterator/particleIterator/ParticleIteratorNonFixedParticles.h"
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
                                struct boundaryConfig boundaryConfigArg = {
                                    {outflow, outflow},
                                    {outflow, outflow},
                                    {outflow, outflow}
                                });


    /**
     * @brief adds particle to container
     * @param particle the particle to add
     */
    void addParticle(const Particle &particle) override;

    /**
     * @brief returns the number of contained particles
     */
    int size() override;

    /**
     * @brief copies this container
     */
    std::unique_ptr<ParticleContainer> copy() override;

    /**
     * @brief returns a ParticlIterator at the first particle
     */
    ParticleIterator begin() override;

    /**
     * @brief returns a ParticleIterator at the last particle
     */
    ParticleIterator end() override;

    /**
     * @brief returns a PairParticleIterator at the first particle pair
     */
    std::unique_ptr<PairParticleIterator> beginPairParticle() override;

    /**
     * @brief returns a PairParticleIterator at the last particle pair
     */
    std::unique_ptr<PairParticleIterator> endPairParticle() override;

    /**
     * @brief returns iterator only non-fixed particles
     */
    ParticleIteratorNonFixedParticles beginNonFixedParticles() override;

    /**
     * @brief returns iterator at end only non-fixed particles
     */
    ParticleIteratorNonFixedParticles endNonFixedParticles() override;

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
     * @brief provides an iterator for iterating through pairs of direct neighbors in a membrane
     * @return the start of the direct neighbor membrane iterator
     */
    PairParticleIteratorMembraneDirectNeighbor beginMembraneDirectNeighbor();

    /**
     * @brief provides an iterator pointing to the end of pairs of direct neighbors in a membrane
     * @return the end of the direct neighbor membrane iterator
     */
    PairParticleIteratorMembraneDirectNeighbor endMembraneDirectNeighbor();

    /**
     * @brief provides an iterator for iterating through pairs of diagonal neighbors in a membrane
     * @return the start of the diagonal neighbor membrane iterator
     */
    PairParticleIteratorMembraneDiagonalNeighbor beginMembraneDiagonalNeighbor();

    /**
     * @brief provides an iterator pointing to the end of pairs of diagonal neighbors in a membrane
     * @return the end of the diagonal neighbor membrane iterator
     */
    PairParticleIteratorMembraneDiagonalNeighbor endMembraneDiagonalNeighbor();

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
     * @brief changes the supplied vector to a periodic one, if the supplied points are on opposite sides of a periodic boundary. If the points aren't on opposite sides, 
     * or the boundary is non-periodic, the supplied vector remains unchanged
     * @param point1 the first point
     * @param point2 the second point
     * @param distanceVector the non-periodic vector that points from point1 to point2
     */
    void getPeriodicDistanceVector(const std::array<double, 3> &point1, const std::array<double, 3> &point2,
                                   std::array<double, 3> &distanceVector);

    /**
     * @brief applies periodic boundaries to the specified coordinate array, so that if the coordinate goes out of
     * bounds on a periodic boundary, it is reinserted at the periodic boundary on the opposite side
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

    /**
     * @brief computes the partitions of the mesh, so that each each thread can be assigned a partition and work on it
     * without interfering with other threads.
     * @param numThreads the number of threads available for the simulation
     */
    void computeMeshPartition(size_t numThreads);

    /**
     * @brief returns mesh (vector of cells)
     */
    std::vector<Cell> &getMesh();

    /**
     * @brief returns particles (vector)
     */
    std::vector<Particle> &getParticles();

    /**
     * @brief returns cell at given id
     * @param idx cell id
     */
    Cell &getCell(int idx);

    /**
     * @brief returns number of cells (3dimensional)
     */
    std::array<size_t, 3> getNumCells();

    /**
     * @brief returns size of a single cell
     */
    std::array<double, 3> getCellSize();

    /**
     * @brief returns cutoff radius
     */
    double getCutoffRadius();

    /*
     * @brief returns particle at given particleIndex
     */
    Particle &getParticle(size_t particleIndex) override;

    /**
     * @brief returns domain size
     */
    std::array<double, 3> getDomainSize();

    /**
     * @brief returns boundary config
     */
    struct boundaryConfig getBoundaryConfig();

    /**
     * @brief returns neighbor cells matrix
     */
    std::vector<std::vector<size_t> > &getNeighborCellsMatrix();

    /**
     * @brief returns mesh partition
     */
    std::pair<std::vector<std::vector<size_t> >, std::vector<std::vector<size_t> > > &getMeshPartition();

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
    std::vector<std::vector<size_t> > neighborCellsMatrix;

    /**
     * @brief a pair of matricies storing the partitioned mesh for race-free multithreading 
     */
    std::pair<std::vector<std::vector<size_t> >, std::vector<std::vector<size_t> > > meshPartition;
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
     * @param particleIdx the particle to correct the position of
     * @returns true if the particle should be removed from its old cell
     */
    bool correctCellMembershipSingleParticle(size_t particleIdx);
};
