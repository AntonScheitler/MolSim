#include "ParticleIterator.h"

ParticleIterator::ParticleIterator(std::vector<Particle>::iterator it) {
    current = it;
}

ParticleIterator::reference ParticleIterator::operator*() {
    return *current;
}

ParticleIterator &ParticleIterator::operator++() {
    ++current;
    return *this;
}

bool ParticleIterator::operator!=(const ParticleIterator &other) {
    return current != other.current;
}