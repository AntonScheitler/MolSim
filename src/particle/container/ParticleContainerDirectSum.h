#include <vector>
#include "../Particle.h"
#include "particle/iterator/particleIterator/ParticleIteratorDirectSum.h"
#include <particle/iterator/pairParticleIterator/PairParticleIteratorDirectSum.h>
#include <particle/container/ParticleContainer.h>

#pragma once

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate particles or particle pairs
 */
class ParticleContainerDirectSum : public ParticleContainer {

    public:
        /**
         * @brief creates an empty ParticleContainer
         * @return the object of an empty ParticleContainer
         */
        ParticleContainerDirectSum();

        /**
         * @brief wraps a vector in a ParticleContainer
         * @param particlesArg the particle vector to be wrapped
         * @return the object of the ParticleContainer
         */
        explicit ParticleContainerDirectSum(const std::vector<Particle> &particlesArg, double averageVelocityArg);

        void addParticle(const Particle &particle) override;
        int size() override;
        double getAverageVelocity() override;
        void setAverageVelocity(double averageVelocityArg) override;
        Particle& getParticle(int index) override;
        ParticleIteratorDirectSum begin();
        ParticleIteratorDirectSum end();
        PairParticleIteratorDirectSum beginPairParticle();
        PairParticleIteratorDirectSum endPairParticle();


    protected:
        /**
         * @brief the particles wrapped by this container
         */
        std::vector<Particle> particles;
        /**
         * @brief the average brownian velocity of the particles
         */
        double averageVelocity;
};
