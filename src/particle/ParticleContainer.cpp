#include <list>
#include "ParticleContainer.h"

class ParticleContainer {

private:

    std::vector<Particle> particles;

public:

    ParticleContainer() {
        this->particles = std::vector<Particle>{};
    }

    ParticleContainer(const std::vector<Particle> &particles) {
        this->particles = particles;
    }

    void addParticle(const Particle &particle) {
        this->particles.push_back(particle);
    }


    class Iterator : public std::iterator<std::forward_iterator_tag, Particle> {

    public:
        Iterator(std::vector<Particle>::iterator it) : it(it) {}

        Iterator& operator++() {
            ++it;
            return *this;
        }

        bool operator!=(const Iterator &other) const { return it != other.it; }

        Particle &operator*() { return *it; }

    private:
        std::vector<Particle>::iterator it;

    };

    Iterator begin() { return Iterator(particles.begin()); }

    Iterator end() { return Iterator(particles.end()); }

    class PairIterator : std::iterator<std::forward_iterator_tag, std::pair<Particle, Particle>> {

    public:
        PairIterator(std::vector<Particle>::iterator it1, std::vector<Particle>::iterator it2) :
                it1(it1), it2(it2) {
            nextPair();
        }

        PairIterator &operator++() {
            ++it2;
            if (it2 == particles.end()) {
                ++it1;
                it2 = std::next(it1);
            }
            nextPair();
            return *this;
        }

        bool operator!=(const PairIterator &other) const {
            return it1 != other.it1 || it2 != other.it2;
        }

        std::pair<Particle, Particle> operator*() const {
            return std::make_pair(*it1, *it2);
        }

    private:

        void nextPair() {
            while (it2 != particles.end() && it1 == it2) {
                ++it2;
            }
        }

        std::vector<Particle>::iterator it1, it2;
        std::vector<Particle> particles;

        PairIterator pairBegin() {
            return PairIterator(particles.begin(), std::next(particles.begin()));
        }

        PairIterator pairEnd() {
            return PairIterator(particles.end(), particles.end());
        }

    };

};
