#include "particle/container/ParticleContainer.h"
#include "particle/container/ParticleContainerLinkedCell.h"
#include <utility>

/**
 * @brief a class wrapping force-computing functions and their utility functions
 */
class ForceComputations {
public:
    /**
     * @brief computes the gravitational forces between the planets and changes them in-place
     * @param particles the planets, for which the gravitational forces need to be computed
     */
    static void computeGravity(ParticleContainer &particles);

    /**
     * @brief computes the lennard jones potential between each pair of molecules in the given ParticleContainer
     * @param particles the ParticleContainer containing the molecules
     * @param epsilonArg epsilon for computation of Lennard Jones Potential
     * @param sigmaArg sigma for computation of Lennard Jones Potential
     */
    static void computeLennardJonesPotential(ParticleContainer &particles, double epsilonArg, double sigmaArg);

    /**
     * @brief computes the lennard jones potential between each pair of molecules in the given ParticleContainer
     * that are not further apart than the cutoff radius
     * @param particles the ParticleContainer containing the molecules
     * @param epsilonArg epsilon for computation of Lennard Jones Potential
     * @param sigmaArg sigma for computation of Lennard Jones Potential
     * @param cutoff the radius past which particles are not considered for the force computation
     */
    static void computeLennardJonesPotentialCutoff(ParticleContainerLinkedCell &particles, double epsilon, double sigma,
                                                   double cutoff);


    /**
     * @brief updates the old force and resets the current force for all particles
     * @param particles the particles to reset the forces of
     */
    static void resetForces(ParticleContainer &particles);

    static void addExternalForces(ParticleContainer &particles, double grav);

    /**
     * @brief computes the repulsion, the boundary particles suffer from the ghost particles
     * @param particles the particles to get the boundary particles from
     * @param epsilon epsilon for computation of Lennard Jones Potential
     * @param sigma sigma for computation of Lennard Jones Potential
     */
    static void computeGhostParticleRepulsion(ParticleContainerLinkedCell &particles, double epsilon, double sigma);

    /**
     * @brief computes the repulsive force between neighboring particles in a membrane
     * @param particles the particles to get the boundary particles from
     * @param epsilon epsilon for computation of Lennard Jones Potential
     * @param sigma sigma for computation of Lennard Jones Potential
     * @param k the stiffness constant for force computation
     * @param r0 the average bond length for force computation
     */
    static void computeMembraneNeighborRepulsion(ParticleContainerLinkedCell &particles, double epsilon, double sigma, double k, double r0);

private:
    /**
     * @brief a helper function for computing the lenndard jones potential between two particles and only applying it, if it is repulsive
     * @param pair the pair of particles to compute the forces between
     * @param epsilon epsilon constant for force computation
     * @param sigma sigma constant for force computation
     */
    static void computeLennardJonesPotentialRepulsiveHelper(std::pair<Particle&, Particle&>& pair, double epsilon, double sigma);

    /**
     * @brief a helper function for computing the harmonic potential between two particles
     * @param pair the pair of particles to compute the forces between
     * @param k the stiffness constant for force computation
     * @param bondLength the bond length for force computation
     */
    static void computeHaromicPotentialHelper(std::pair<Particle&, Particle&>& pair, double k, double bondLength);
};
