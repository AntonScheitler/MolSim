#include "particle/container/ParticleContainer.h"
#include "particle/container/ParticleContainerLinkedCell.h"
#include <map>

/**
 * @brief a class wrapping force-computing functions and their utility functions
 */
class ForceComputations {
    public:
        /**
         * @brief computes the gravitational forces between the planets and changes them in-place
         * @param particles the planets, for which the gravitational forces need to be computed
         */
        static void computeGravity(ParticleContainer& particles);
        /**
         * @brief computes the lennard jones potential between each pair of molecules in the given ParticleContainer
         * @param particles the ParticleContainer containing the molecules
         * @param epsilonArg epsilon for computation of Lennard Jones Potential
         * @param sigmaArg sigma for computation of Lennard Jones Potential
         */
        static void computeLennardJonesPotential(ParticleContainer& particles, double epsilonArg, double sigmaArg);

        /**
         * @brief computes the lennard jones potential between each pair of molecules in the given ParticleContainer
         * that are not further apart than the cutoff radius
         * @param particles the ParticleContainer containing the molecules
         * @param epsilonArg epsilon for computation of Lennard Jones Potential
         * @param sigmaArg sigma for computation of Lennard Jones Potential
         * @param cutoff the radius past which particles are not considered for the force computation
         */
        static void computeLennardJonesPotentialCutoff(ParticleContainer &particles, double epsilon, double sigma, double cutoff);


        /**
         * @brief updates the old force and resets the current force for all particles
         * @param particles the particles to reset the forces of
         */
        static void resetForces(ParticleContainer& particles);

        static void addExternalForces(ParticleContainer& particles, double grav);

        /**
         * @brief computes the repulsion, the boundary particles suffer from the ghost particles
         * @param particles the particles to get the boundary particles from
         * @param epsilon epsilon for computation of Lennard Jones Potential
         * @param sigma sigma for computation of Lennard Jones Potential
         */
        static void computeGhostParticleRepulsion(ParticleContainerLinkedCell& particles, double epsilon, double sigma);

/*private:
     static std::map<std::pair<double, double> , double > epsilons;
    static std::map<std::pair<double, double> , double > sigmas;*/
};
