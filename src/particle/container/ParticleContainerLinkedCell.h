#include <particle/Particle.h>
#include <particle/container/ParticleContainer.h>
#include <particle/iterator/particleIterator/ParticleIteratorLinkedCell.h>
#include <vector>

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate
 * particles or particle pairs
 */
class ParticleContainerLinkedCell : ParticleContainer {

    public:
        /**
         * @brief creates an empty ParticleContainer
         * @return the object of an empty ParticleContainer
         */
        ParticleContainerLinkedCell();

        /**
         * @brief wraps a vector in a ParticleContainer
         * @param particlesArg the particle vector to be wrapped
         * @return the object of the ParticleContainer
         */
        explicit ParticleContainerLinkedCell(const std::vector<Particle> &particlesArg, std::vector<double> domainSize, double cutoffRadius);

        void addParticle(const Particle &particle) override;
        int size() override;
        double getAverageVelocity() override;
        void setAverageVelocity(double averageVelocityArg) override;
        Particle &getParticle(int index) override;
        ParticleIteratorLinkedCell begin();
        ParticleIteratorLinkedCell end();
        PairParticleIterator beginPairParticle();
        PairParticleIterator endPairParticle();
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

    protected:
        /**
         * @brief mesh contains all grid cells
         */
        std::vector<Cell> mesh;
        /**
         * @brief the average brownian velocity of the particles
         */
        double averageVelocity;
        std::vector<double>
            domainDims;      // size of the domain of this particleContainer
        double cutoffRadius; // = cutoffRadius
        std::array<double, 3> cellSize; // dimensions of one cell
        std::array<size_t, 3> numCells; // number of cells per dimensions
};
