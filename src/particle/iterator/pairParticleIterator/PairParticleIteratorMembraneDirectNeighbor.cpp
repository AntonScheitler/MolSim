#include "particle/iterator/pairParticleIterator/PairParticleIteratorMembraneDirectNeighbor.h"
PairParticleIteratorMembraneDirectNeighbor::PairParticleIteratorMembraneDirectNeighbor(std::vector<Particle>::iterator it, 
        std::vector<Particle>::iterator end, std::vector<Particle>& particlesArg): particles(particlesArg) {
    curr = it;
    currNeighborPos = right;
    end = end;
    stepToNextViableParticle();
}


PairParticleIterator::reference PairParticleIteratorMembraneDirectNeighbor::operator*() {
    return {*curr, particles[curr->getNeighborIdx(currNeighborPos)]};
}

PairParticleIteratorMembraneDirectNeighbor& PairParticleIteratorMembraneDirectNeighbor::operator++() {
    if (currNeighborPos == top) {
        ++curr;
    }
    currNeighborPos = currNeighborPos == right ? top : right;
    stepToNextViableParticle();

    return (*this);
}


void PairParticleIteratorMembraneDirectNeighbor::stepToNextViableParticle() {
    while (curr->getNeighborIdx(currNeighborPos) == -1 && curr != end) {
        if (currNeighborPos == top) {
            ++curr;
        }
        currNeighborPos = currNeighborPos == right ? top : right;
    }
}

bool PairParticleIteratorMembraneDirectNeighbor::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorMembraneDirectNeighbor*>(&other);
    if (casted) {
        return curr != casted->curr || currNeighborPos != casted->currNeighborPos;
    }
    return true;
}
