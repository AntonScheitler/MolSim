#pragma once
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
     */
    static void computeLennardJonesPotential(ParticleContainer &particles);

    /**
     * @brief computes the lennard jones potential between each pair of molecules in the given ParticleContainer
     * that are not further apart than the cutoff radius
     * @param particles the ParticleContainer containing the molecules
     * @param cutoff the radius past which particles are not considered for the force computation
     */
    static void computeLennardJonesPotentialCutoff(ParticleContainerLinkedCell &particles, double cutoff);

    /**
     * @brief an alternative to computeLennardJonesPotentialCutoff with an iteration optimized for parallelism 
     * @param particles the ParticleContainer containing the molecules
     * @param cutoff the radius past which particles are not considered for the force computation
     */
    static void computeLennardJonesPotentialCutoffCellIter(ParticleContainerLinkedCell& particles, double cutoff);

    /**
     * @brief updates the old force and resets the current force for all particles
     * @param particles the particles to reset the forces of
     */
    static void resetForces(ParticleContainer &particles);

    /**
     * @brief adds an external gravity force to all given particles
     * @param particles all particles that the force should be applied to
     * @param grav the gravitational acceleration in x,y and z direction
     */
    static void addExternalForces(ParticleContainer &particles, std::array<double, 3> grav);

    /**
     * @brief computes the repulsion, the boundary particles suffer from the ghost particles
     * @param particles the particles to get the boundary particles from
     */
    static void computeGhostParticleRepulsion(ParticleContainerLinkedCell &particles);

    /**
     * @brief computes the repulsive force between neighboring particles in a membrane
     * @param particles the particles to get the boundary particles from
     * @param k the stiffness constant for force computation
     * @param r0 the average bond length for force computation
     */
    static void computeMembraneNeighborForce(ParticleContainerLinkedCell &particles, double k, double r0);

    /**
     * @brief applies a custom force vector to a selection of particles in the container
     * @param particles the particle container of which a selection of particles will have the force applied to them
     * @param indices the indices of the particles to apply the force to
     * @param f the custom force vector to apply to the particles
     */
    static void applyCustomForceVector(ParticleContainerLinkedCell& particles, std::vector<size_t> indices, std::array<double, 3> f);

private:
    /**
     * @brief a helper function for computing the lenndard jones potential between two particles and only applying it, if it is repulsive
     * @param pair the pair of particles to compute the forces between
     * @param epsilon epsilon constant for force computation
     * @param sigma sigma constant for force computation
     */
    static void computeLennardJonesPotentialRepulsiveHelper(std::pair<Particle&, Particle&>& pair, double epsilon, double sigma);

    /**
     * @brief a helper function for computing the lenndard jones potential between two particles
     * @param particles the container for the particles that the pair is a part of
     * @param pair the pair of particles to compute the forces between
     * @param cutoff the cutoff radius past which no forces are computed
     */
    static void computeLennardJonesPotentialCutoffHelper(ParticleContainerLinkedCell& particles, std::pair<Particle&, Particle&>& pair, double cutoff);

    /**
     * @brief a helper function for computing the harmonic potential between two particles
     * @param pair the pair of particles to compute the forces between
     * @param k the stiffness constant for force computation
     * @param bondLength the bond length for force computation
     */
    static void computeHaromicPotentialHelper(std::pair<Particle&, Particle&>& pair, double k, double bondLength);
};
