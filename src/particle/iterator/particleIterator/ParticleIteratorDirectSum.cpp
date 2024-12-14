#include "ParticleIteratorDirectSum.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"


ParticleIteratorDirectSum::ParticleIteratorDirectSum(std::vector<Particle>::iterator it) {
    current = it;
}

ParticleIteratorDirectSum::reference ParticleIteratorDirectSum::operator*() {
    return *current;
}

ParticleIteratorDirectSum &ParticleIteratorDirectSum::operator++() {
    ++current;
    return *this;
}

bool ParticleIteratorDirectSum::operator!=(const ParticleIterator &other) {
    auto casted = dynamic_cast<const ParticleIteratorDirectSum *>(&other);
    if (casted) {
        return current != casted->current;
    }
    return true;
}
