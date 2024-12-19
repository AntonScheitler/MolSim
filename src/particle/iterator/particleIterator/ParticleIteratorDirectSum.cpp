#include "ParticleIteratorDirectSum.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"


ParticleIteratorDirectSum::ParticleIteratorDirectSum(std::vector<Particle>::iterator it, std::vector<Particle>::iterator endArg) {
    current = it;
    end = endArg;
    while (current != end && !(current->getActive())) {
        ++current;
    }
}

ParticleIteratorDirectSum::reference ParticleIteratorDirectSum::operator*() {
    return *current;
}

ParticleIteratorDirectSum &ParticleIteratorDirectSum::operator++() {
    do {
        ++current;
    } while (current != end && !(current->getActive()));
    return *this;
}

bool ParticleIteratorDirectSum::operator!=(const ParticleIterator &other) {
    auto casted = dynamic_cast<const ParticleIteratorDirectSum *>(&other);
    if (casted) {
        return current != casted->current;
    }
    return true;
}
