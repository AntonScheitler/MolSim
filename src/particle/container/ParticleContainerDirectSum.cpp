#include "../container/ParticleContainerDirectSum.h"
#include <vector>

ParticleContainerDirectSum::ParticleContainerDirectSum() {
    particles = std::vector<Particle>{};
    averageVelocity = 0.1;
}

ParticleContainerDirectSum::ParticleContainerDirectSum(const std::vector<Particle> &particlesArg, double averageVelocityArg=0.1) {
    particles = particlesArg;
    averageVelocity = averageVelocityArg;
}

void ParticleContainerDirectSum::addParticle(const Particle &particle) {
    particles.push_back(particle);
    averageVelocity = 0.1;
}

int ParticleContainerDirectSum::size() {
    return particles.size();
}

Particle& ParticleContainerDirectSum::getParticle(int index) {
    return particles.at(index);
}

double ParticleContainerDirectSum::getAverageVelocity() {
    return averageVelocity;
}

void ParticleContainerDirectSum::setAverageVelocity(double averageVelocityArg) {
    averageVelocity = averageVelocityArg;
}

// single particle iterator
ParticleIterator ParticleContainerDirectSum::begin() {
    return particles.begin();
}

ParticleIterator ParticleContainerDirectSum::end() {
    return particles.end();
}

// pair particle iterator
PairParticleIterator ParticleContainerDirectSum::beginPairParticle() {
    if (particles.empty()) {
        return {particles.begin(), particles.begin(), particles.end()};
    }
    return {particles.begin(), ++(particles.begin()), particles.end()};
}

PairParticleIterator ParticleContainerDirectSum::endPairParticle() {
    if (particles.empty()) {
        return {particles.end(), particles.end(), particles.end()};
    }
    return {--(particles.end()), particles.end(), particles.end()};
}
