#pragma once

#include <particle/Particle.h>
#include <iterator>


/**
 * @brief an iterator which enables iteration over pairs of particles. The elements of those pairs are always
 * distinct ordering is not considered. This means, that a specific pair <a, b> will only appear once when
 * iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using reference = std::pair<Particle &, Particle &>;

    PairParticleIterator() = default;

    virtual ~PairParticleIterator() = default;

    /**
     * @brief Dereference this PairParticleIterator, i.e. get the current pair of Particles
     * @return std::pair of Particles
     */
    virtual reference operator*() = 0;

    /**
     * @brief Increment this iterator, i.e. get the next distinct pair of particles
     * @return this PairParticleIterator updated
     */
    virtual PairParticleIterator &operator++() = 0;

    /**
     * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator
     * @param other other PairParticleIterator
     * @return True if iterators not equal
     */
    virtual bool operator!=(const PairParticleIterator &other) = 0;
};
