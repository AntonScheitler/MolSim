#include "ParticleIteratorNonFixedParticles.h"

ParticleIteratorNonFixedParticles::ParticleIteratorNonFixedParticles(std::vector<Particle>::iterator it,
                                                                     std::vector<Particle>::iterator endArg) {
    current = it;
    end = endArg;
    while (current != end && (!(current->getActive()) || current->isFixed())) {
        ++current;
    }
}

ParticleIteratorNonFixedParticles::reference ParticleIteratorNonFixedParticles::operator*() {
    return *current;
}

ParticleIteratorNonFixedParticles &ParticleIteratorNonFixedParticles::operator++() {
    do {
        ++current;
    } while (current != end && (!(current->getActive()) || current->isFixed()));
    return *this;
}

bool ParticleIteratorNonFixedParticles::operator!=(const ParticleIteratorNonFixedParticles &other) {
    return current != other.current;
}
