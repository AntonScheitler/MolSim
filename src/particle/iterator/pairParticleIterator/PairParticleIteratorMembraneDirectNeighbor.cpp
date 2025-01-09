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

PairParticleIteratorMembraneDirectNeighbor & PairParticleIteratorMembraneDirectNeighbor::operator++() {
    // TODO
}


void PairParticleIteratorMembraneDirectNeighbor::stepToNextViableParticle() {
    // TODO
}

bool PairParticleIteratorMembraneDirectNeighbor::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorMembraneDirectNeighbor*>(&other);
    if (casted) {
        return curr != casted->curr || currNeighborPos != casted->currNeighborPos;
    }
    return true;
}
