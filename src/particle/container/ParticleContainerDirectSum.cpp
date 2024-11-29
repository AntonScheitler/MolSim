#include "particle/Particle.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIteratorDirectSum.h"
#include <algorithm>
#include <memory>
#include <particle/container/ParticleContainerDirectSum.h>
#include <vector>
#include <particle/container/ParticleContainerDirectSum.h>

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
std::unique_ptr<ParticleIterator> ParticleContainerDirectSum::begin() {
    return std::unique_ptr<ParticleIterator>(new ParticleIteratorDirectSum(particles.begin()));
}

std::unique_ptr<ParticleIterator> ParticleContainerDirectSum::end() {
    return std::unique_ptr<ParticleIterator>(new ParticleIteratorDirectSum(particles.end()));
}

// pair particle iterator
std::unique_ptr<PairParticleIterator> ParticleContainerDirectSum::beginPairParticle() {
    if (particles.empty()) {
        return std::unique_ptr<PairParticleIterator>(new PairParticleIteratorDirectSum(particles.begin(), particles.begin(), particles.end()));
    }
    return std::unique_ptr<PairParticleIterator>(new PairParticleIteratorDirectSum(particles.begin(), ++(particles.begin()), particles.end()));
}

std::unique_ptr<PairParticleIterator> ParticleContainerDirectSum::endPairParticle() {
    if (particles.empty()) {
        return std::unique_ptr<PairParticleIterator>(new PairParticleIteratorDirectSum(particles.end(), particles.end(), particles.end()));
    }
    return std::unique_ptr<PairParticleIterator>(new PairParticleIteratorDirectSum(--(particles.end()), particles.end(), particles.end()));
}
