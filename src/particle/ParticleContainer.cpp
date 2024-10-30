#include <vector>
#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() {
    this->particles = std::vector<Particle>{};
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &particles) {
    this->particles = particles;
}

void ParticleContainer::addParticle(const Particle &particle) {
    this->particles.push_back(particle);
}

ParticleContainer::Iterator ParticleContainer::begin() { return Iterator(particles.begin()); }

ParticleContainer::Iterator ParticleContainer::end() { return Iterator(particles.end()); }

ParticleContainer::PairIterator ParticleContainer::pairBegin() {
    return {particles.begin(), std::next(particles.begin())};
}

ParticleContainer::PairIterator ParticleContainer::pairEnd() {
    return {particles.end(), particles.end()};
}

int ParticleContainer::size() {
    return particles.size();
}

ParticleContainer::Iterator::Iterator(std::vector<Particle>::iterator it) {
    this->it = it;
}

ParticleContainer::Iterator &ParticleContainer::Iterator::operator++() {
    ++it;
    return *this;
}

bool ParticleContainer::Iterator::operator!=(const Iterator &other) const { return it != other.it; }

Particle &ParticleContainer::Iterator::operator*() { return *it; }


ParticleContainer::PairIterator::PairIterator(std::vector<Particle>::iterator it1, std::vector<Particle>::iterator it2) : it1(it1), it2(it2) {
    nextPair();
}

ParticleContainer::PairIterator &ParticleContainer::PairIterator::operator++() {
    ++it2;
    if (it2 == particles.end()) {
        ++it1;
        it2 = std::next(it1);
    }
    nextPair();
    return *this;
}

bool ParticleContainer::PairIterator::operator!=(const PairIterator &other) const {
    return it1 != other.it1 || it2 != other.it2;
}

std::pair<Particle, Particle> ParticleContainer::PairIterator::operator*() const {
    return std::make_pair(*it1, *it2);
}

void ParticleContainer::PairIterator::nextPair() {
    while (it2 != particles.end() && it1 == it2) {
        ++it2;
    }
}
