#include <list>
#include <vector>
#include "Particle.h"

#pragma once

/**
 *  The class ParticleContainer groups multiple Particles together and allows single iteration as well as
 *  iteration over all Particle pairs.
 */
class ParticleContainer {

public:

    ParticleContainer();

    /**
     * Constructs a ParticleContainer with the given Particle vector as elements.
     * @param particles Elements of the ParticleContainer.
     */
    explicit ParticleContainer(const std::vector<Particle> &particles);

    void addParticle(const Particle &particle);
    int size();

    /**
     * Iteration over single elements of the ParticleContainer.
     */
    class Iterator : public std::iterator<std::forward_iterator_tag, Particle> {

    public:
        /**
         * Creates a single Iterator to iterate over all elements of the ParticleContainer one a a time.
         * @param it
         */
        explicit Iterator(std::vector<Particle>::iterator it);

        Iterator &operator++();

        bool operator!=(const Iterator &other) const;

        Particle &operator*();

    private:
        std::vector<Particle>::iterator it;

    }; // class Iterator

    Iterator begin();

    Iterator end();

    /**
     * Iteration over all pairs of the elements of the ParticleContainer.
     */
    class PairIterator : std::iterator<std::forward_iterator_tag, std::pair<Particle, Particle>> {

    public:
        /**
         * Creates a pair iterator to iterate over all pairs of elements.
         * @param it1
         * @param it2
         */
        PairIterator(std::vector<Particle>::iterator it1, std::vector<Particle>::iterator it2);

        PairIterator &operator++();

        bool operator!=(const PairIterator &other) const;

        std::pair<Particle, Particle> operator*() const;

    private:

        void nextPair();

        std::vector<Particle>::iterator it1, it2;
        std::vector<Particle> particles;

    }; // class PairIterator

    PairIterator pairBegin();

    PairIterator pairEnd();

private:

    std::vector<Particle> particles;

}; // class ParticleContainer
