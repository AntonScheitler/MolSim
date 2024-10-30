#include "particle/ParticleContainer.h"
#include <vector>

ParticleContainer::ParticleContainer() {
    particles = std::vector<Particle>{};
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &particles_arg) {
    particles = particles_arg;
}

void ParticleContainer::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

int ParticleContainer::size() {
    return particles.size();
} 

ParticleContainer::ParticleIterator ParticleContainer::begin() {
    return particles.begin();
}

ParticleContainer::ParticleIterator ParticleContainer::end() {
    return particles.end();
}

ParticleContainer::ParticleIterator ParticleContainer::beginParticle() {
    return particles.begin();
}

ParticleContainer::ParticleIterator ParticleContainer::endParticle() {
    return particles.end();
}

ParticleContainer::PairParticleIterator ParticleContainer::beginPairParticle() {
    if (particles.empty()) {
        return {particles.begin(), particles.begin(), particles.end()};
    }
    return {particles.begin(), ++(particles.begin()), particles.end()};
}

ParticleContainer::PairParticleIterator ParticleContainer::endPairParticle() {
    if (particles.empty()) {
        return {particles.end(), particles.end(), particles.end()};
    }
    return {--(particles.end()), particles.end(), particles.end()};
}

ParticleContainer::ParticleIterator::ParticleIterator(std::vector<Particle>::iterator it) {
    current = it;
}


ParticleContainer::ParticleIterator::reference ParticleContainer::ParticleIterator::operator*() {
    return *current;
}

ParticleContainer::ParticleIterator& ParticleContainer::ParticleIterator::operator++() {
    ++current;
    return *this;
}

bool ParticleContainer::ParticleIterator::operator!=(const ParticleContainer::ParticleIterator& other) {
    return current != other.current;
}

ParticleContainer::PairParticleIterator::PairParticleIterator(std::vector<Particle>::iterator first_arg,
        std::vector<Particle>::iterator second_arg,
        std::vector<Particle>::iterator end_arg) {
    first = first_arg;
    second = second_arg;
    end = end_arg;
}

ParticleContainer::PairParticleIterator::reference ParticleContainer::PairParticleIterator::operator*() {
    return {*first, *second};
}

ParticleContainer::PairParticleIterator& ParticleContainer::PairParticleIterator::operator++() {
    if (second == (end - 1)) {
        first++;
        second = first;
    }
    second++;
    return *this;
}

bool ParticleContainer::PairParticleIterator::operator!=(const ParticleContainer::PairParticleIterator& other) {
    return first != other.first || second != other.second || end != other.end;
}
