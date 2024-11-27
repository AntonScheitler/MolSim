#include "PairParticleIteratorDirectSum.h"


PairParticleIteratorDirectSum::PairParticleIteratorDirectSum(std::vector<Particle>::iterator firstArg,
                                           std::vector<Particle>::iterator secondArg,
                                           std::vector<Particle>::iterator endArg) {
    first = firstArg;
    second = secondArg;
    end = endArg;
}

PairParticleIteratorDirectSum::reference PairParticleIteratorDirectSum::operator*() {
    return {*first, *second};
}

PairParticleIteratorDirectSum &PairParticleIteratorDirectSum::operator++() {
    if (second == (end - 1)) {
        first++;
        second = first;
    }
    second++;
    return *this;
}

bool PairParticleIteratorDirectSum::operator!=(const PairParticleIteratorDirectSum &other) {
    return first != other.first || second != other.second || end != other.end;
}
