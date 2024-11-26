#include <vector>
#include <particle/Particle.h>
#include <particle/cell/Cell.h>
#include "ParticleContainer.h"
#include "spdlogConfig.h"

enum BoundaryCondition {
    outflowing,
    reflecting
};

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate particles or particle pairs
 */
class ParticleContainerLinkedCell : ParticleContainer {

public:

    /**
     * @brief wraps a vector in a ParticleContainer
     * @param particlesArg the particle vector to be wrapped
     * @return the object of the ParticleContainer
     */
    explicit ParticleContainerLinkedCell(std::array<double, 3> domainDimsArg, double cutoffRadiusArg);

    void addParticle(const Particle &particle) override;

    int size() override;

    double getAverageVelocity() override;

    void setAverageVelocity(double averageVelocityArg) override;

    Particle &getParticle(int x) override;

    ParticleIterator begin() override;

    ParticleIterator end() override;

    PairParticleIterator beginPairParticle() override;

    PairParticleIterator endPairParticle() override;

    Cell &getCell(int index);

    /**
     * @brief initializes the mesh which is a grid of cells
     * @param domainSize the 3d size of the domain
     * @param cutoffRadius the cutoffRadius (used for determining for cell size)
     */
    void initMesh();

    /**
     * @brief converts the discrete (index) coordinates of a cell in the mesh to the index at which the cell is in the mesh
     * vector
     * @param coord
     * @return
     */
    int discreteCoordsToIndex(std::array<int, 3> coord);

    /**
     * @brief converts the continuous coordinates (which can be anywhere in the mesh) to the index of the cell in
     * the mesh vector where these coordinates (e.g. of a particle) lie in
     * @param coord
     * @return
     */
    int continuousCoordsToIndex(std::array<double, 3> coord);

    /**
     *  @brief checks the position of one particle and moves it to other cells if needed or
     *  removes it completely from the mesh
     */
    void correctParticleCell(Particle& p);
    /**
     * @brief checks the positions of all particles in the mesh and corrects them
     */
    void correctAllParticlesCell();

protected:
    /**
     * @brief mesh contains all grid cells
     */
    std::vector<Cell> mesh;
    /**
     * @brief the average brownian velocity of the particles
     */
    double averageVelocity;
    std::array<double, 3> domainDims; // size of the domain of this particleContainer
    double cutoffRadius; // = cutoffRadius
    std::array<double, 3> cellSize; // dimensions of one cell
    std::array<int, 3> numCells; // number of cells per dimensions
    BoundaryCondition boundaryCondition;

};
