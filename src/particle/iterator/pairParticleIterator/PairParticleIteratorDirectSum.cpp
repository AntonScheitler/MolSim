#include "PairParticleIteratorDirectSum.h"
#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"


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

bool PairParticleIteratorDirectSum::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorDirectSum *>(&other);
    if (casted) {
        return first != casted->first || second != casted->second || end != casted->end;
    }
    return true;
}
