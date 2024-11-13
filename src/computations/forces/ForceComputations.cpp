#include <array>
#include <cmath>

#include "../../utils/ArrayUtils.h"
#include "particle/ParticleContainer.h"
#include "ForceComputations.h"
#include "spdlogConfig.h"

void ForceComputations::computeGravity(ParticleContainer &particles) {
    for (auto it = particles.beginPairParticle(); it != particles.endPairParticle(); ++it) {
        std::pair<Particle &, Particle &> pair = *it;
        std::array<double, 3> newForce = {0, 0, 0};

        // gravity computation according to the formula
        std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.second.getX(), pair.first.getX(),
                                                                             std::minus<>());
        double distance = ArrayUtils::L2Norm(distanceVector);
        if (distance == 0) continue;
        double coefficient = (pair.first.getM() * pair.second.getM()) / std::pow(distance, 3);
        newForce = ArrayUtils::elementWiseScalarOp(coefficient, distanceVector, std::multiplies<>());
        pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), newForce, std::plus<>()));

        // the gravitational force that affects the other planet is the same that affects the first but pointed to
        // the opposite direction.
        // therefore it needs to be multiplied by -1
        std::array<double, 3> revNewForce = ArrayUtils::elementWiseScalarOp(-1, newForce, std::multiplies<>());
        pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revNewForce, std::plus<>()));
    }
}

void ForceComputations::computeLennardJonesPotential(ParticleContainer &particles, double epsilonArg, double sigmaArg) {
    // hardcoded for now (later dynamic)
    double epsilon = epsilonArg;
    double sigma = sigmaArg;

    // iterate through all pairs of particles and calculate lennard-jones potential
    for (auto it = particles.beginPairParticle(); it != particles.endPairParticle(); ++it) {
        std::pair<Particle &, Particle &> pair = *it;

        std::array<double, 3> distanceVector = ArrayUtils::elementWisePairOp(pair.first.getX(), pair.second.getX(), std::minus<>());
        double distance = ArrayUtils::L2Norm(distanceVector);
        if (distance == 0) continue;

        double factor = (-24.0 * epsilon) / std::pow(distance, 2) * (std::pow(sigma / distance, 6) - 2 * std::pow(sigma / distance, 12));

        std::array<double, 3> force = ArrayUtils::elementWiseScalarOp(factor, distanceVector, std::multiplies<>());
        pair.first.setF(ArrayUtils::elementWisePairOp(pair.first.getF(), force, std::plus<>()));
        std::array<double, 3> revForce = ArrayUtils::elementWiseScalarOp(-1, force, std::multiplies<>());
        pair.second.setF(ArrayUtils::elementWisePairOp(pair.second.getF(), revForce, std::plus<>()));
    }
}

void ForceComputations::resetForces(ParticleContainer &particles) {
    for (Particle &particle: particles) {
        particle.setOldF(particle.getF());
        particle.setF({0, 0, 0});
    }
}



