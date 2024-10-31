#include "Particle.h"
#include <iterator>
#include <vector>

#pragma once

/**
 * @brief wrapper for a vector of particles, enabling iteration over separate particles or particle pairs
 */
class ParticleContainer {

public:
    /**
     * @brief creates an empty ParticleContainer
     * @return the object of an empty ParticleContainer
     */
    ParticleContainer();

    /**
     * @brief wraps a vector in a ParticleContainer
     * @param particles the particle vector to be wrapped
     * @return the object of the ParticleContainer
     */
    explicit ParticleContainer(const std::vector<Particle> &particles);

    /**
     * @brief adds a particle to the container
     * @param particle the particle to be added
     */
    void addParticle(const Particle &particle);

    /**
     * @brief returns the number of particles in the ParticleContainer
     * @return the number of particles in the ParticleContainer
     */
    int size();

    /**
     * @brief an iterator which enables iteration over each particle separately
     */
    class ParticleIterator {

    public:
        using iterator_category = std::forward_iterator_tag;
        using reference = Particle &;

        /**
         * @brief create an instance of a ParticleIterator
         * @param it the vector iterator to build the ParticleIterator upon
         * @return an instance of a ParticleIterator
         */
        ParticleIterator(std::vector<Particle>::iterator it);

        reference operator*();

        ParticleIterator &operator++();

        bool operator!=(const ParticleIterator &other);

    private:
        std::vector<Particle>::iterator current;
    };

    /**
     * @brief returns a ParticleIterator which points to the beginning of the ParticleContainer. This makes iteration over
     * separate particles the default when using a for-each loop
     * @return a ParticleIterator which points to the beginning of the ParticleContainer
     */
    ParticleIterator begin();

    /**
     * @brief returns a ParticleIterator which points to the end of the ParticleContainer. This makes iteration over
     * separate particles the default when using a for-each loop
     * @return a ParticleIterator which points to the end of the ParticleContainer
     */
    ParticleIterator end();

    /**
     * @brief an iterator which enables iteration over pairs of particles. The elements of those pairs are always
     * distinct ordering is not considered. This means, that a specific pair <a, b> will only appear once when
     * iterating. <b, a> will never show itself in an iteration
     */
    class PairParticleIterator {

    public:
        using iterator_category = std::forward_iterator_tag;
        using reference = std::pair<Particle &, Particle &>;

        /**
         * @brief create an instance of a PairParticleIterator
         * @param first the vector iterator which will be used to generate the first element in the pair
         * @param second the vector iterator which will be used to generate the second element in the pair
         * @param end a vector iterator pointing to the end of the particle vector, which is needed to determine when the
         * final pair has been reached
         * @return an instance of a PairParticleIterator
         */
        PairParticleIterator(std::vector<Particle>::iterator first,
                             std::vector<Particle>::iterator second,
                             std::vector<Particle>::iterator end);

        reference operator*();

        PairParticleIterator &operator++();

        bool operator!=(const PairParticleIterator &other);

    private:
        std::vector<Particle>::iterator first;
        std::vector<Particle>::iterator second;
        std::vector<Particle>::iterator end;
    };

    /**
     * @brief returns a PairParticleIterator which points to the first pair of the ParticleContainer
     * @return a PairParticleIterator which points to the first pair of the ParticleContainer
     */
    PairParticleIterator beginPairParticle();

    /**
     * @brief returns a PairParticleIterator which points to the theoretical pair after the last pair of the ParticleContainer
     * @return a PairParticleIterator which points to the theoretical pair after the last pair of the ParticleContainer
     */
    PairParticleIterator endPairParticle();

protected:
    std::vector<Particle> particles;
};
