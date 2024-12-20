#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <particle/Particle.h>
#include <vector>

#pragma once

/**
 * @brief an iterator which enables iteration over pairs of particles. The elements of those pairs are always
 * distinct ordering is not considered. This means, that a specific pair <a, b> will only appear once when
 * iterating. <b, a> will never show itself in an iteration
 */
class PairParticleIteratorDirectSum : public PairParticleIterator {

public:
    /**
     * @brief create an instance of a PairParticleIterator
     * @param firstArg the vector iterator which will be used to generate the first element in the pair
     * @param secondArg the vector iterator which will be used to generate the second element in the pair
     * @param endArg a vector iterator pointing to the end of the particle vector, which is needed to determine when the
     * final pair has been reached
     * @return an instance of a PairParticleIterator
     */
    PairParticleIteratorDirectSum(std::vector<Particle>::iterator firstArg,
                                  std::vector<Particle>::iterator secondArg,
                                  std::vector<Particle>::iterator endArg);

    /**
     * @brief Dereference this PairParticleIterator, i.e. get the current pair of Particles
     * @return std::pair of Particles
     */
    reference operator*() override;

    /**
     * @brief Increment this iterator, i.e. get the next distinct pair of particles
     * @return this PairParticleIterator updated
     */
    PairParticleIteratorDirectSum &operator++() override;

    /**
     * @brief check whether this PairParticleIterator is not equal to another PairParticleIterator
     * @param other other PairParticleIterator
     * @return True if iterators not equal
     */
    bool operator!=(const PairParticleIterator &other) override;

private:
    /**
     * @brief an iterator marking the first particle of a pair
     */
    std::vector<Particle>::iterator first;
    /**
     * @brief an iterator marking the second particle of a pair
     */
    std::vector<Particle>::iterator second;
    /**
     * @brief an iterator marking the end of the particle vector
     */
    std::vector<Particle>::iterator end;
};
