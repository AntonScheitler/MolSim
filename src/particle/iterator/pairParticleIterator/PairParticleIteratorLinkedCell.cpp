#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "utils/ArrayUtils.h"
#include <array>
#include <cstddef>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>

PairParticleIteratorLinkedCell::PairParticleIteratorLinkedCell( std::vector<Cell>::iterator it, std::vector<Cell>::iterator endArg, std::vector<Cell> &meshArg, std::array<size_t, 3> numCellsArg) {
    completedParticles.clear();
    currentCellIdx = {0, 0, 0};
    mesh = meshArg;
    numCells = numCellsArg;
    currentCell = it;
    end = endArg;
    currentStepToViableCell(false);
}

void PairParticleIteratorLinkedCell::incrementCurrCellIdx() {
    if (currentCellIdx[0] == numCells[0] - 1) {
        if (currentCellIdx[1] == numCells[1] - 1) {
            currentCellIdx[2] = (currentCellIdx[2] + 1) % numCells[2];
        }
        currentCellIdx[1] = (currentCellIdx[1] + 1) % numCells[1];
    }
    currentCellIdx[0] = (currentCellIdx[0] + 1) % numCells[0];
}

std::vector<Cell> PairParticleIteratorLinkedCell::getNeighborCells() {
    std::vector<Cell> neighborCells = std::vector<Cell>();
    for (int z = -1; z < 2; z++) {
        for (int y = -1; y < 2; y++) {
            for (int x = -1; x < 2; x++) {
                std::array<int, 3> neighborCoords = ArrayUtils::elementWisePairOp(
                        currentCellIdx, {x, y, z}, std::plus<>());
                if (neighborCoords[0] < 0 || neighborCoords[1] < 0 || neighborCoords[2] < 0 || 
                        neighborCoords[0] >= numCells[0] || neighborCoords[1] >= numCells[1] || neighborCoords[2] >= numCells[2]) continue;

                int neighborIdx = neighborCoords[0] + (neighborCoords[1] * numCells[0]) + (neighborCoords[2] * numCells[0] * numCells[1]);
                neighborCells.push_back(mesh[neighborIdx]);
            }
        }
    }
    return neighborCells;
}


PairParticleIteratorLinkedCell::reference PairParticleIteratorLinkedCell::operator*() {
    return {*currentParticle, *neighborParticle};
}

PairParticleIteratorLinkedCell &PairParticleIteratorLinkedCell::operator++() {
    // step to the next viable neighbor particle
    ++neighborParticle;
    neighborStepToViableParticle();
    // go to the next neighbor cell if all neighbor particles have been iterated
    // through
    if (neighborParticle == neighborCell->getParticles().end()) {
        // step to the next non-empty neighbor cell which contains viable particles
        neighborStepToViableCell(true);
        while (neighborCell == neighborEnd) {
            // add the current particle to the set of particles for which all pairs
            // have been computed
            completedParticles.insert(*currentParticle);
            ++currentParticle;
            currentStepToViableParticle();
            if (currentParticle == (*currentCell).getParticles().end()) {
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

bool PairParticleIteratorLinkedCell::operator!=(
        const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorLinkedCell *>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    }
    return true;
}

void PairParticleIteratorLinkedCell::currentStepToViableCell(bool stepBefore) {
    do {
        do {
            if (stepBefore) {
                ++currentCell;
                incrementCurrCellIdx();
            }
            stepBefore = true;
            while (currentCell != end && currentCell->getParticles().empty()) {
                ++currentCell;
                incrementCurrCellIdx();
            }
            if (currentCell == end) {
                return;
            }
            currentParticle = currentCell->getParticles().begin();
            currentStepToViableParticle();
        } while (currentCell != end && currentParticle == currentCell->getParticles().end());
        if (currentCell == end) {
            return;
        }
        neighborCellsVector = getNeighborCells();
        neighborCell = neighborCellsVector.begin();
        neighborEnd = neighborCellsVector.end();
        neighborStepToViableCell(false);
    } while(neighborCell == neighborEnd);
}

void PairParticleIteratorLinkedCell::currentStepToViableParticle() {
    while (currentParticle != currentCell->getParticles().end() &&
            completedParticles.count(*currentParticle) > 0) {
        ++currentParticle;
    }
}

void PairParticleIteratorLinkedCell::neighborStepToViableCell(bool stepBefore) {
    do {
        if (stepBefore) ++neighborCell;
        stepBefore = true;
        while (neighborCell != neighborEnd && neighborCell->getParticles().size() == 0) {
            ++neighborCell;
        }
        if (neighborCell == neighborEnd) {
            return;
        }
        neighborParticle = neighborCell->getParticles().begin();
        neighborStepToViableParticle();
    } while (neighborCell != neighborEnd &&
            neighborParticle == neighborCell->getParticles().end());
}

void PairParticleIteratorLinkedCell::neighborStepToViableParticle() {
    while (neighborParticle != neighborCell->getParticles().end() &&
            ((*currentParticle) == (*neighborParticle) || (completedParticles.count(*neighborParticle) > 0))) {
        ++neighborParticle;
    }
}
