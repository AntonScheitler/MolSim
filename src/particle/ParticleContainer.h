#include "Particle.h"
#include <iterator>
#include <vector>

#pragma once

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate particles and particle pairs
 */
class ParticleContainer {

    public:
        ParticleContainer();
        ParticleContainer(const std::vector<Particle> &particles);

        void addParticle(const Particle &particle);
        int size();

        class ParticleIterator {

            public:
                using iterator_category = std::forward_iterator_tag;
                using reference = Particle&;

                ParticleIterator(std::vector<Particle>::iterator it);

                reference operator*();
                ParticleIterator& operator++();
                bool operator!=(const ParticleIterator& other);
            private:
                std::vector<Particle>::iterator current;
        };

        ParticleIterator begin();
        ParticleIterator end();

        ParticleIterator beginParticle();
        ParticleIterator endParticle();

        class PairParticleIterator {

            public:
                using iterator_category = std::forward_iterator_tag;
                using reference = std::pair<Particle&, Particle&>;

                PairParticleIterator(std::vector<Particle>::iterator first,
                        std::vector<Particle>::iterator second,
                        std::vector<Particle>::iterator end);

                reference operator*();
                PairParticleIterator& operator++();
                bool operator!=(const PairParticleIterator& other);
            private:
                std::vector<Particle>::iterator first;
                std::vector<Particle>::iterator second;
                std::vector<Particle>::iterator end;
        };

        PairParticleIterator beginPairParticle();
        PairParticleIterator endPairParticle();

    protected:
        std::vector<Particle> particles;
};
