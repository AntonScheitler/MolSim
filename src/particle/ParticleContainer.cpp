#include "particle/ParticleContainer.h"
#include <vector>

ParticleContainer::ParticleContainer() {
    particles = std::vector<Particle>{};
    averageVelocity = 0.1;
}

ParticleContainer::ParticleContainer(const std::vector<Particle> &particlesArg, double averageVelocityArg=0.1) {
    particles = particlesArg;
    averageVelocity = averageVelocityArg;
}

void ParticleContainer::addParticle(const Particle &particle) {
    particles.push_back(particle);
    averageVelocity = 0.1;
}

int ParticleContainer::size() {
    return particles.size();
}

double ParticleContainer::getAverageVelocity() {
    return averageVelocity;
}

void ParticleContainer::setAverageVelocity(double averageVelocityArg) {
    averageVelocity = averageVelocityArg;
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
