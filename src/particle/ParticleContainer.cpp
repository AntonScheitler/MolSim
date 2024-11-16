#include "particle/ParticleContainer.h"
#include <vector>

ParticleContainer::ParticleContainer() {
    particles = std::vector<Particle>{};
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &particlesArg) {
    particles = particlesArg;
}

void ParticleContainer::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

int ParticleContainer::size() {
    return particles.size();
}

// single particle iterator
ParticleIterator ParticleContainer::begin() {
    return particles.begin();
}

ParticleIterator ParticleContainer::end() {
    return particles.end();
}

// pair particle iterator
PairParticleIterator ParticleContainer::beginPairParticle() {
    if (particles.empty()) {
        return {particles.begin(), particles.begin(), particles.end()};
    }
    return {particles.begin(), ++(particles.begin()), particles.end()};
}

PairParticleIterator ParticleContainer::endPairParticle() {
    if (particles.empty()) {
        return {particles.end(), particles.end(), particles.end()};
    }
    return {--(particles.end()), particles.end(), particles.end()};
}
