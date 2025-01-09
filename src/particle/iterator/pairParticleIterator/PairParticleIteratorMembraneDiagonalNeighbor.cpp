#include "particle/iterator/pairParticleIterator/PairParticleIteratorMembraneDiagonalNeighbor.h"
PairParticleIteratorMembraneDiagonalNeighbor::PairParticleIteratorMembraneDiagonalNeighbor(std::vector<Particle>::iterator it, 
        std::vector<Particle>::iterator end, std::vector<Particle>& particlesArg): particles(particlesArg) {
    curr = it;
    currNeighborPos = topLeft;
    end = end;
    stepToNextViableParticle();
}


PairParticleIterator::reference PairParticleIteratorMembraneDiagonalNeighbor::operator*() {
    return {*curr, particles[curr->getDiagNeighborIdx(currNeighborPos)]};
}

PairParticleIteratorMembraneDiagonalNeighbor & PairParticleIteratorMembraneDiagonalNeighbor::operator++() {
    // TODO
}


void PairParticleIteratorMembraneDiagonalNeighbor::stepToNextViableParticle() {
    // TODO
}

bool PairParticleIteratorMembraneDiagonalNeighbor::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorMembraneDiagonalNeighbor*>(&other);
    if (casted) {
        return curr != casted->curr || currNeighborPos != casted->currNeighborPos;
    }
    return true;
}
