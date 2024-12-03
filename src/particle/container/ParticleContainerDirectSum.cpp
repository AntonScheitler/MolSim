#include <particle/container/ParticleContainerDirectSum.h>
#include <memory>
#include <vector>
#include "particle/Particle.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIteratorDirectSum.h"


ParticleContainerDirectSum::ParticleContainerDirectSum() {
    particles = std::vector<Particle>{};
}

ParticleContainerDirectSum::ParticleContainerDirectSum(const std::vector<Particle> &particlesArg) {
    particles = particlesArg;
}

void ParticleContainerDirectSum::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

int ParticleContainerDirectSum::size() {
    return particles.size();
}

std::unique_ptr<ParticleContainer> ParticleContainerDirectSum::copy() {
    return std::make_unique<ParticleContainerDirectSum>(*this);
}

Particle &ParticleContainerDirectSum::getParticle(int index) {
    return particles.at(index);
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
        return std::unique_ptr<PairParticleIterator>(
                new PairParticleIteratorDirectSum(particles.begin(), particles.begin(), particles.end()));
    }
    return std::unique_ptr<PairParticleIterator>(
            new PairParticleIteratorDirectSum(particles.begin(), ++(particles.begin()), particles.end()));
}

std::unique_ptr<PairParticleIterator> ParticleContainerDirectSum::endPairParticle() {
    if (particles.empty()) {
        return std::unique_ptr<PairParticleIterator>(
                new PairParticleIteratorDirectSum(particles.end(), particles.end(), particles.end()));
    }
    return std::unique_ptr<PairParticleIterator>(
            new PairParticleIteratorDirectSum(--(particles.end()), particles.end(), particles.end()));
}
