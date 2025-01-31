#include <array>
#include <cmath>
#include <functional>
#include <utility>
#include "../../utils/ArrayUtils.h"
#include "particle/container/ParticleContainer.h"
#include "ForceComputations.h"
#include "spdlogConfig.h"
#include <omp.h>

#ifdef _OPENMP
#else
    #define omp_get_thread_num() 0
#endif

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

void ForceComputations::computeLennardJonesPotential(ParticleContainer &particles) {
    double epsilon;
    double sigma;
    // iterate through all pairs of particles and calculate lennard-jones potential
    for (auto it = particles.beginPairParticle(); *it != *(particles.endPairParticle()); it->operator++()) {
        std::pair<Particle &, Particle &> pair = **it;

        sigma = (pair.first.getSigma() != pair.second.getSigma())
                    ? ((pair.first.getSigma() + pair.second.getSigma()) / 2)
                    : pair.first.getSigma();
        epsilon = (pair.first.getEpsilon() != pair.second.getEpsilon())
                      ? (sqrt(pair.first.getEpsilon() * pair.second.getSigma()))
                      : pair.first.getEpsilon();

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

void ForceComputations::computeLennardJonesPotentialCutoff(ParticleContainerLinkedCell &particles, double cutoff,
                                                           size_t numThreads) {
    double epsilon;
    double sigma;
    std::vector<std::pair<Particle &, Particle &> > particlePairs = {};
    for (auto it = particles.beginPairParticle(); *it != *(particles.endPairParticle()); it->operator++()) {
        particlePairs.push_back(**it);
    }
    // iterate through all pairs of particles and calculate lennard-jones potential

#pragma omp parallel for num_threads(numThreads)
    for (size_t i = 0; i < particlePairs.size(); i++) {
        std::pair<Particle &, Particle &> pair = particlePairs[i];
        computeLennardJonesPotentialCutoffHelperPair(particles, pair, cutoff, true);
    }
}


void ForceComputations::computeLennardJonesPotentialCutoffCellIter(ParticleContainerLinkedCell &particles,
                                                                   double cutoff, size_t numThreads) {
    // iterate through every cell
#pragma omp parallel for num_threads(numThreads)
    for (size_t cellIdx = 0; cellIdx < particles.getMesh().size(); cellIdx++) {
        computeLennardJonesPotentialCutoffHelperCell(particles, cellIdx, cutoff, true);
    }
}

void ForceComputations::computeLennardJonesPotentialCutoffMeshPart(ParticleContainerLinkedCell &particles,
                                                                   double cutoff, size_t numThreads) {
    auto partitionPair = particles.getMeshPartition();
    auto partitions = partitionPair.first;
    auto borderPartitions = partitionPair.second;

    // iterate through all of the partitions
#pragma omp parallel num_threads(numThreads)
    {
        auto partition = partitions[omp_get_thread_num()];
        for (size_t cellIdx: partition) {
            computeLennardJonesPotentialCutoffHelperCell(particles, cellIdx, cutoff, false);
        }
    }
    // perform computations for the border cells as well
#pragma omp parallel num_threads(numThreads)
    {
        auto borderPartition = borderPartitions[omp_get_thread_num()];
        for (size_t cellIdx: borderPartition) {
            computeLennardJonesPotentialCutoffHelperCell(particles, cellIdx, cutoff, false);
        }
    }
}


void ForceComputations::resetForces(ParticleContainer &particles, size_t numThreads) {
#pragma omp parallel for num_threads(numThreads)
    for (size_t i = 0; i < particles.size(); i++) {
        Particle &particle = particles.getParticle(i);
        if (particle.isFixed() || !(particle.getActive())) continue;
        particle.setOldF(particle.getF());
        particle.setF({0, 0, 0});
    }
}


void ForceComputations::addExternalForces(ParticleContainer &particles, std::array<double, 3> grav, size_t numThreads) {
#pragma omp parallel for num_threads(numThreads)
    for (size_t i = 0; i < particles.size(); i++) {
        Particle &particle = particles.getParticle(i);
        if (particle.isFixed() || !(particle.getActive())) continue;
        std::array<double, 3> newForce = particle.getF();
        newForce[0] += particle.getM() * grav[0];
        newForce[1] += particle.getM() * grav[1];
        newForce[2] += particle.getM() * grav[2];
        particle.setF(newForce);
    }
}

void ForceComputations::computeGhostParticleRepulsion(ParticleContainerLinkedCell &particles) {
    double epsilon;
    double sigma;
    std::vector<std::pair<Particle &, Particle &> > pairs = {};

    for (auto it = particles.beginPairGhost(); it != particles.endPairGhost(); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        epsilon = pair.first.getEpsilon();
        sigma = pair.first.getSigma();
        computeLennardJonesPotentialRepulsiveHelper(pair, epsilon, sigma, true);
    }
}

void ForceComputations::computeMembraneNeighborForce(ParticleContainerLinkedCell &particles, double k, double r0) {
    for (auto it = particles.beginMembraneDirectNeighbor(); it != particles.endMembraneDirectNeighbor(); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        computeHaromicPotentialHelper(pair, k, r0);
    }

    // compute harmonic potential for diagonal neighbors
    for (auto it = particles.beginMembraneDiagonalNeighbor(); it != particles.endMembraneDiagonalNeighbor(); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        computeHaromicPotentialHelper(pair, k, sqrt(2.0) * r0);
    }

    // compute repulsive force between all particles
    double epsilon;
    double sigma;
    for (auto it = particles.beginPairParticle(); *it != *(particles.endPairParticle()); ++*it) {
        std::pair<Particle &, Particle &> pair = **it;
        epsilon = pair.first.getEpsilon();
        sigma = pair.first.getSigma();
        computeLennardJonesPotentialRepulsiveHelper(pair, epsilon, sigma, false);
    }
}

void ForceComputations::computeLennardJonesPotentialCutoffHelperPair(ParticleContainerLinkedCell &particles,
                                                                     std::pair<Particle &, Particle &> &pair,
                                                                     double cutoff, bool critical) {
    double sigma = (pair.first.getSigma() != pair.second.getSigma())
                       ? ((pair.first.getSigma() + pair.second.getSigma()) / 2)
                       : pair.first.getSigma();
    double epsilon = (pair.first.getEpsilon() != pair.second.getEpsilon())
                         ? (sqrt(pair.first.getEpsilon() * pair.second.getSigma()))
                         : pair.first.getEpsilon();

    std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.first.getX(), pair.second.getX(),
                                                                         std::minus<>());
    particles.getPeriodicDistanceVector(pair.first.getX(), pair.second.getX(), distanceVector);
    double distance = ArrayUtils::L2Norm(distanceVector);
    // don't consider particles which are further apart than the cutoff radius
    if (distance == 0 || distance > cutoff) return;
    double dist = std::pow(sigma / distance, 2);

    double factor = (-24.0 * epsilon) / std::pow(distance, 2) * (std::pow(dist, 3) - 2 * std::pow(dist, 6));

    std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
    std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
    if (critical) {
#pragma omp critical
        {
            pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
            pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
        }
    } else {
        pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
        pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
    }
}

void ForceComputations::computeLennardJonesPotentialCutoffHelperCell(ParticleContainerLinkedCell &particles,
                                                                     size_t cellIdx, double cutoff, bool critical) {
    std::vector<size_t> &currParticleIndices = particles.getMesh()[cellIdx].getParticlesIndices();
    std::vector<size_t> &neighborCellsIndices = particles.getNeighborCellsMatrix()[cellIdx];
    // iterate through every particle in this cell
    for (size_t currParticleIdx: currParticleIndices) {
        // first iterate through all the other particles in the same cell
        for (size_t neighborParticleIdx: currParticleIndices) {
            if (neighborParticleIdx <= currParticleIdx) continue;
            std::pair<Particle &, Particle &> pair = {
                particles.getParticle(currParticleIdx), particles.getParticle(neighborParticleIdx)
            };
            computeLennardJonesPotentialCutoffHelperPair(particles, pair, cutoff, critical);
        }
        // then iterate through all of the other neighbor cells
        for (size_t neighborCellIdx: neighborCellsIndices) {
            if (neighborCellIdx == cellIdx) continue;
            // and all of the particles in those neighbor cells
            for (size_t neighborParticleIdx: particles.getMesh()[neighborCellIdx].getParticlesIndices()) {
                std::pair<Particle &, Particle &> pair = {
                    particles.getParticle(currParticleIdx), particles.getParticle(neighborParticleIdx)
                };
                computeLennardJonesPotentialCutoffHelperPair(particles, pair, cutoff, critical);
            }
        }
    }
}

void ForceComputations::computeLennardJonesPotentialRepulsiveHelper(std::pair<Particle &, Particle &> &pair,
                                                                    double epsilon, double sigma, bool critical) {
    std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(
        pair.first.getX(), pair.second.getX(), std::minus<>());
    double distance = ArrayUtils::L2Norm(distanceVector);
    double sigmaDivDistance = sigma / distance;

    // don't compute force if it is not repulsive
    if (distance == 0 || distance >= (std::pow(2.0, 1.0 / 6.0) * sigma)) return;
    double factor = (-24.0 * epsilon) / std::pow(distance, 2) *
                    (std::pow(sigmaDivDistance, 6) - 2 * std::pow(sigmaDivDistance, 12));
    std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
    std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
    if (critical) {
#pragma omp critical
        {
            pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
            pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
        }
    }
    pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
    pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
}

void ForceComputations::computeHaromicPotentialHelper(std::pair<Particle &, Particle &> &pair, double k,
                                                      double bondLength) {
    std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(
        pair.second.getX(), pair.first.getX(), std::minus<>());
    double distance = ArrayUtils::L2Norm(distanceVector);
    double coefficient = k * (distance - bondLength) * (1.0 / distance);

    std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(coefficient, distanceVector, std::multiplies<>());
    pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
    std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
    pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
}

void ForceComputations::applyCustomForceVector(ParticleContainerLinkedCell &particles, std::vector<size_t> indices,
                                               std::array<double, 3> f) {
    for (auto idx: indices) {
        Particle &particle = particles.getParticles()[idx];
        particle.setF(ArrayUtils::elementWisePairOp(particle.getF(), f, std::plus<>()));
    }
}
