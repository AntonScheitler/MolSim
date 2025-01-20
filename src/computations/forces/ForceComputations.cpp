#include <array>
#include <cmath>
#include <functional>
#include <utility>
#include "../../utils/ArrayUtils.h"
#include "particle/container/ParticleContainer.h"
#include "ForceComputations.h"
#include "spdlogConfig.h"

void ForceComputations::computeGravity(ParticleContainer &particles) {
    for (auto it = particles.beginPairParticle(); *it != *(particles.endPairParticle()); it->operator++()) {
        std::pair<Particle &, Particle &> pair = **it;
        std::array<double, 3> newForce = {0, 0, 0};

        // gravity computation according to the formula
        std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.second.getX(), pair.first.getX(),
                                                                             std::minus<>());
        double distance = ArrayUtils::L2Norm(distanceVector);
        if (distance == 0) continue;
        double coefficient = (pair.first.getM() * pair.second.getM()) / std::pow(distance, 3);
        newForce = ArrayUtils::elementWiseScalarOp(coefficient, distanceVector, std::multiplies<>());
        pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), newForce, std::plus<>()));

        // the gravitational force that affects the other particle is the same that affects the first but pointed to
        // the opposite direction.
        // therefore it needs to be multiplied by -1
        std::array<double, 3> revNewForce = ArrayUtils::elementWiseScalarOp(-1, newForce, std::multiplies<>());
        pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revNewForce, std::plus<>()));
    }
}

void ForceComputations::computeLennardJonesPotential(ParticleContainer &particles, double epsilon, double sigma) {
    // iterate through all pairs of particles and calculate lennard-jones potential
    for (auto it = particles.beginPairParticle(); *it != *(particles.endPairParticle()); it->operator++()) {
        std::pair<Particle &, Particle &> pair = **it;

        if (pair.first.getSigma() != pair.second.getSigma()) {
            sigma = (pair.first.getSigma() + pair.second.getSigma()) / 2;
        }
        if (pair.first.getEpsilon() != pair.second.getEpsilon()) {
            epsilon = sqrt(pair.first.getEpsilon() * pair.second.getSigma());
        }

        std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.first.getX(), pair.second.getX(),
                                                                             std::minus<>());
        double distance = ArrayUtils::L2Norm(distanceVector);
        if (distance == 0) continue;

        double sigmaDivDistance = sigma / distance;

        double factor = (-24.0 * epsilon) / std::pow(distance, 2) *
                        (std::pow(sigmaDivDistance, 6) - 2 * std::pow(sigmaDivDistance, 12));


        std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
        pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
        std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
        pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
    }
}

void ForceComputations::computeLennardJonesPotentialCutoff(ParticleContainerLinkedCell &particles, double epsilon,
                                                           double sigma, double cutoff) {
    // iterate through all pairs of particles and calculate lennard-jones potential
    for (auto it = particles.beginPairParticle(); *it != *(particles.endPairParticle()); it->operator++()) {
        std::pair<Particle &, Particle &> pair = **it;
        if (pair.first.getSigma() != pair.second.getSigma()) {
            sigma = (pair.first.getSigma() + pair.second.getSigma()) / 2;
        }
        if (pair.first.getEpsilon() != pair.second.getEpsilon()) {
            epsilon = sqrt(pair.first.getEpsilon() * pair.second.getSigma());
        }

        std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.first.getX(), pair.second.getX(),
                                                                             std::minus<>());
        particles.getPeriodicDistanceVector(pair.first.getX(), pair.second.getX(), distanceVector);
        double distance = ArrayUtils::L2Norm(distanceVector);
        // don't consider particles which are further apart than the cutoff radius
        if (distance == 0 || distance > cutoff) continue;
        double dist = std::pow(sigma / distance, 2);

        double factor = (-24.0 * epsilon) / std::pow(distance, 2) * (std::pow(dist, 3) - 2 * std::pow(dist, 6));

        std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
        pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
        std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
        pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
    }
}

void ForceComputations::resetForces(ParticleContainer &particles) {
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle& particle = *it;
        particle.setOldF(particle.getF());
        particle.setF({0, 0, 0});
    }
}


void ForceComputations::addExternalForces(ParticleContainer &particles, std::array<double, 3> grav) {
    for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
        Particle &particle = *it;
        std::array<double, 3> newForce = particle.getF();
        newForce[0] += particle.getM() * grav[0];
        newForce[1] += particle.getM() * grav[1];
        newForce[2] += particle.getM() * grav[2];
        particle.setF(newForce);
    }
}

void ForceComputations::computeGhostParticleRepulsion(ParticleContainerLinkedCell &particles, double epsilon, double sigma) {
    for (auto it = particles.beginPairGhost(); it != particles.endPairGhost(); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        computeLennardJonesPotentialRepulsiveHelper(pair, epsilon, sigma);
    }
}

void ForceComputations::computeMembraneNeighborForce(ParticleContainerLinkedCell &particles, double epsilon, double sigma, double k, double r0) {
    for (auto it = particles.beginMembraneDirectNeighbor(); it != particles.endMembraneDirectNeighbor(); ++it) {
        std::pair<Particle&, Particle&> pair = *it;
        computeLennardJonesPotentialRepulsiveHelper(pair, epsilon, sigma);
        computeHaromicPotentialHelper(pair, k, r0);
    }

    for (auto it = particles.beginMembraneDiagonalNeighbor(); it != particles.endMembraneDiagonalNeighbor(); ++it) {
        std::pair<Particle&, Particle&> pair = *it;
        computeLennardJonesPotentialRepulsiveHelper(pair, epsilon, sigma);
        computeHaromicPotentialHelper(pair, k, sqrt(2.0) * r0);
    }
}


void ForceComputations::computeLennardJonesPotentialRepulsiveHelper(std::pair<Particle&, Particle&>& pair, double epsilon, double sigma) {
    std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.first.getX(), pair.second.getX(), std::minus<>());
    double distance = ArrayUtils::L2Norm(distanceVector);
    double sigmaDivDistance = sigma / distance;

    // don't compute force if it is not repulsive
    if (distance == 0 || distance >= (std::pow(2.0, 1.0 / 6.0) * sigma)) return;
    double factor = (-24.0 * epsilon) / std::pow(distance, 2) *
                    (std::pow(sigmaDivDistance, 6) - 2 * std::pow(sigmaDivDistance, 12));
    std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
    pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
    std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
    pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
}

void ForceComputations::computeHaromicPotentialHelper(std::pair<Particle&, Particle&>& pair, double k, double bondLength) {
    std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.second.getX(), pair.first.getX(), std::minus<>());
    double distance = ArrayUtils::L2Norm(distanceVector);
    double coefficient = k * (distance - bondLength) * (1.0 / distance);

    std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(coefficient, distanceVector, std::multiplies<>());
    pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
    std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
    pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
}

void ForceComputations::applyCustomForceVector(ParticleContainerLinkedCell& particles, std::vector<size_t> indices, std::array<double, 3> f) {
    for (auto idx : indices) {
        Particle& particle = particles.getParticles()[idx];
        particle.setF(ArrayUtils::elementWisePairOp(particle.getF(), f, std::plus<>()));
    }
}

