#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "spdlogConfig.h"
#include <cstddef>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>
#include <unordered_set>


PairParticleIteratorLinkedCell::PairParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator endArg, std::vector<Cell> &meshArg,
            std::vector<Particle>& particlesArg, std::vector<std::vector<size_t>>& neighborCellsMatrixArg): 
                mesh(meshArg), particles(particlesArg), neighborCellsMatrix(neighborCellsMatrixArg) {
    completedParticles.clear();
    currentCellIdx = 0;
    currentCell = it;
    end = endArg;
    currentStepToViableCell(false);
}

PairParticleIteratorLinkedCell::reference PairParticleIteratorLinkedCell::operator*() {
    return {particles[*currentParticleIdx], particles[*neighborParticleIdx]};
}

PairParticleIteratorLinkedCell &PairParticleIteratorLinkedCell::operator++() {
    // step to the next viable neighbor particle
    ++neighborParticleIdx;
    neighborStepToViableParticle();
    // go to the next neighbor cell if all neighbor particles have been iterated
    // through
    if (neighborParticleIdx == mesh[*neighborCell].getParticlesIndices().end()) {
        // step to the next non-empty neighbor cell which contains viable particles
        neighborStepToViableCell(true);
        while (neighborCell == neighborEnd) {
            // add the current particle to the set of particles for which all pairs
            // have been computed
            completedParticles.insert(*currentParticleIdx);
            ++currentParticleIdx;
            if (currentParticleIdx == (*currentCell).getParticlesIndices().end()) {
                currentStepToViableCell(true);
                return *this;
            }
            neighborCell = neighborCellsVector.begin();
            neighborEnd = neighborCellsVector.end();
            neighborStepToViableCell(false);
        }
    }
    return *this;
}

bool PairParticleIteratorLinkedCell::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorLinkedCell *>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    }
    return true;
}

void PairParticleIteratorLinkedCell::currentStepToViableCell(bool stepBefore) {
    do {
        if (stepBefore) {
            ++currentCell;
            currentCellIdx++;
        }
        stepBefore = true;
        while (currentCell != end && currentCell->getParticlesIndices().empty()) {
            ++currentCell;
            currentCellIdx++;
        }
        if (currentCell == end) {
            return;
        }
        completedParticles.clear();
        currentParticleIdx = currentCell->getParticlesIndices().begin();
        neighborCellsVector = neighborCellsMatrix[currentCellIdx];
        neighborCell = neighborCellsVector.begin();
        neighborEnd = neighborCellsVector.end();
        neighborStepToViableCell(false);
    } while (neighborCell == neighborEnd);
}

void PairParticleIteratorLinkedCell::neighborStepToViableCell(bool stepBefore) {
    do {
        if (stepBefore) ++neighborCell;
        stepBefore = true;
        while (neighborCell != neighborEnd && mesh[*neighborCell].getParticlesIndices().empty()) {
            ++neighborCell;
        }
        if (neighborCell == neighborEnd) {
            return;
        }
        neighborParticleIdx = mesh[*neighborCell].getParticlesIndices().begin();
        neighborStepToViableParticle();
    } while (neighborCell != neighborEnd && neighborParticleIdx == mesh[*neighborCell].getParticlesIndices().end());
}

void PairParticleIteratorLinkedCell::neighborStepToViableParticle() {
    while (neighborParticleIdx != mesh[*neighborCell].getParticlesIndices().end() &&
               ((*currentParticleIdx) == (*neighborParticleIdx) || (completedParticles.count(*neighborParticleIdx) > 0))) {
        ++neighborParticleIdx;
    }
}
