#include "PairParticleIterator.h"


PairParticleIterator::PairParticleIterator(std::vector<Particle>::iterator firstArg,
                                           std::vector<Particle>::iterator secondArg,
                                           std::vector<Particle>::iterator endArg) {
    first = firstArg;
    second = secondArg;
    end = endArg;
}

PairParticleIterator::reference PairParticleIterator::operator*() {
    return {*first, *second};
}

PairParticleIterator &PairParticleIterator::operator++() {
    if (second == (end - 1)) {
        first++;
        second = first;
    }
    second++;
    return *this;
}

bool PairParticleIterator::operator!=(const PairParticleIterator &other) {
    return first != other.first || second != other.second || end != other.end;
}
