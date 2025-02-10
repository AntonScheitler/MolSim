#include <particle/container/ParticleContainerDirectSum.h>
#include <memory>
#include <vector>
#include "particle/Particle.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "particle/iterator/pairParticleIterator/PairParticleIteratorDirectSum.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"


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

Particle &ParticleContainerDirectSum::getParticle(size_t index) {
    return particles.at(index);
}

// single particle iterator
ParticleIterator ParticleContainerDirectSum::begin() {
    return ParticleIterator(particles.begin(), particles.end());
}

ParticleIterator ParticleContainerDirectSum::end() {
    return ParticleIterator(particles.end(), particles.end());
}

// pair particle iterator
std::unique_ptr<PairParticleIterator> ParticleContainerDirectSum::beginPairParticle() {
    if (particles.empty()) {
        return std::make_unique<PairParticleIteratorDirectSum>(
            PairParticleIteratorDirectSum(particles.begin(), particles.begin(), particles.end()));
    }
    return std::make_unique<PairParticleIteratorDirectSum>(
        PairParticleIteratorDirectSum(particles.begin(), ++(particles.begin()), particles.end()));
}

std::unique_ptr<PairParticleIterator> ParticleContainerDirectSum::endPairParticle() {
    if (particles.empty()) {
        return std::make_unique<PairParticleIteratorDirectSum>(
            PairParticleIteratorDirectSum(particles.end(), particles.end(), particles.end()));
    }
    return std::make_unique<PairParticleIteratorDirectSum>(
        PairParticleIteratorDirectSum(--(particles.end()), particles.end(), particles.end()));
}

ParticleIteratorNonFixedParticles ParticleContainerDirectSum::beginNonFixedParticles() {
    return ParticleIteratorNonFixedParticles(particles.begin(), particles.end());
}

ParticleIteratorNonFixedParticles ParticleContainerDirectSum::endNonFixedParticles() {
    return ParticleIteratorNonFixedParticles(particles.end(), particles.end());
}
