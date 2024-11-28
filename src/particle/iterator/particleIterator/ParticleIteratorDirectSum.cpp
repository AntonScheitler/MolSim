#include "ParticleIteratorDirectSum.h"

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

bool ParticleIteratorDirectSum::operator!=(const ParticleIteratorDirectSum &other) {
    return current != other.current;
}
