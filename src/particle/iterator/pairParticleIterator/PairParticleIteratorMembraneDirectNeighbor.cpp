#include "PairParticleIteratorMembraneDirectNeighbor.h"

#include "spdlogConfig.h"

PairParticleIteratorMembraneDirectNeighbor::PairParticleIteratorMembraneDirectNeighbor(std::vector<Particle>::iterator it, 
        std::vector<Particle>::iterator end, std::vector<Particle>& particlesArg): particles(particlesArg) {
    SPDLOG_DEBUG("  initialize PairParticleIteratorMembraneDirectNeighbor");
    curr = it;
    currNeighborPos = right;
    this->end = end;
    SPDLOG_DEBUG("  initialized variables");

    stepToNextViableParticle();
    SPDLOG_DEBUG("  finished stepToNextViableParticle");

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

    while (curr != end && curr->getNeighborIdx(currNeighborPos) == -1) {

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
