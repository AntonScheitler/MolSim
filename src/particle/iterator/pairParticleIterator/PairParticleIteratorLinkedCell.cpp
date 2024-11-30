#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <array>
#include <cstddef>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>

PairParticleIteratorLinkedCell::PairParticleIteratorLinkedCell( std::vector<Cell>::iterator it, std::vector<Cell>::iterator endArg, std::vector<Cell> &meshArg, std::array<size_t, 3> numCellsArg) {
    completedPairs.clear();
    currentCellIdx = 0;
    mesh = meshArg;
    numCells = numCellsArg;
    currentCell = it;
    end = endArg;

    currentStepToViableCell();
    if (currentCell == end) {
        return;
    }

    // get iterator and end of iterator for neighbors of the current cell
    neighborCellsVector = getNeighborCells();
    neighborCell = neighborCellsVector.begin();
    neighborEnd = neighborCellsVector.end();
    neighborStepToViableCell();
    while (neighborCell == neighborEnd) {
        // add the current particle to the set of particles for which all pairs have been computed
        completedPairs.insert(*currentParticle);
        currentParticle++;
        currentStepToViableParticle();
        if (currentParticle == (*currentCell).getParticles().end()) {
            currentCell++;
            currentStepToViableCell();
            if (currentCell == end) {
                return;
            }
            neighborCellsVector = getNeighborCells();
        }
        neighborCell = neighborCellsVector.begin();
        neighborEnd = neighborCellsVector.end();
        neighborStepToViableCell();
    }
}

std::vector<Cell> PairParticleIteratorLinkedCell::getNeighborCells() {
    std::vector<Cell> neighborCells = std::vector<Cell>();
    for (int z = -1; z < 2; z++) {
        for (int y = -1; y < 2; y++) {
            for (int x = -1; x < 2; x++) {
                int neighborIdx = currentCellIdx + x + (y * numCells[0]) +
                    (z * numCells[0] * numCells[1]);
                if (neighborIdx >= 0 && neighborIdx < mesh.size()) {
                    neighborCells.push_back(mesh[neighborIdx]);
                }
            }
        }
    }
    return neighborCells;
}

PairParticleIteratorLinkedCell::reference
PairParticleIteratorLinkedCell::operator*() {
    return {*currentParticle, *neighborParticle};
}

PairParticleIteratorLinkedCell &PairParticleIteratorLinkedCell::operator++() {
    // step to the next viable neighbor particle
    neighborParticle++;
    neighborStepToViableParticle();
    // go to the next neighbor cell if all neighbor particles have been iterated through
    if (neighborParticle == neighborCell->getParticles().end()) {
        // step to the next non-empty neighbor cell which contains viable particles
        neighborCell++;
        neighborStepToViableCell();
        while (neighborCell == neighborEnd) {
            // add the current particle to the set of particles for which all pairs have been computed
            completedPairs.insert(*currentParticle);
            ++currentParticle;
            currentStepToViableParticle();
            if (currentParticle == (*currentCell).getParticles().end()) {
                ++currentCell;
                currentStepToViableCell();
                if (currentCell == end) {
                    return *this;
                }
                neighborCellsVector = getNeighborCells();
            }
            neighborCell = neighborCellsVector.begin();
            neighborEnd = neighborCellsVector.end();
            neighborStepToViableCell();
        }
    }
    return *this;
}

bool PairParticleIteratorLinkedCell::operator!=(
        const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorLinkedCell *>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    }
    return true;
}

void PairParticleIteratorLinkedCell::currentStepToViableCell() {
    do {
        while (currentCell != end && currentCell->getParticles().size() == 0) {
            ++currentCell;
            currentCellIdx++;
        }
        if (currentCell == end) {
            return;
        }
        currentParticle = currentCell->getParticles().begin();
        currentStepToViableParticle();
    } while (currentCell != end && currentParticle == currentCell->getParticles().end());
}

void PairParticleIteratorLinkedCell::currentStepToViableParticle() {
    while (currentParticle != currentCell->getParticles().end() && completedPairs.count(*currentParticle) > 0) {
        ++currentParticle;
    }
}

void PairParticleIteratorLinkedCell::neighborStepToViableCell() {
    do {
        while (neighborCell != neighborEnd && neighborCell->getParticles().size() == 0) {
            ++neighborCell;
        }
        if (neighborCell == neighborEnd) {
            return;
        }
        neighborParticle = neighborCell->getParticles().begin();
        neighborStepToViableParticle();
    } while (neighborCell != neighborEnd && neighborParticle == neighborCell->getParticles().end());
}

void PairParticleIteratorLinkedCell::neighborStepToViableParticle() {
    while (neighborParticle != neighborCell->getParticles().end() &&
            ((*currentParticle) == (*neighborParticle) || (completedPairs.count(*neighborParticle) > 0))) {
        ++neighborParticle;
    }
}
