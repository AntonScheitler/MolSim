#include "particle/iterator/particleIterator/ParticleIterator.h"

ParticleIterator::ParticleIterator(std::vector<Particle>::iterator it, std::vector<Particle>::iterator endArg) {
    current = it;
    end = endArg;
    while (current != end && !(current->getActive())) {
        ++current;
    }
}

ParticleIterator::reference ParticleIterator::operator*() {
    return *current;
}

ParticleIterator &ParticleIterator::operator++() {
    do {
        ++current;
    } while (current != end && !(current->getActive()));
    return *this;
}

bool ParticleIterator::operator!=(const ParticleIterator &other) {
    return current != other.current;
}
