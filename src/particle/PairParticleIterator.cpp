#include "PairParticleIterator.h"


PairParticleIterator::PairParticleIterator(std::vector<Particle>::iterator first_arg,
                                           std::vector<Particle>::iterator second_arg,
                                           std::vector<Particle>::iterator end_arg) {
    first = first_arg;
    second = second_arg;
    end = end_arg;
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
