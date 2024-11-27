#include "utils/ArrayUtils.h"
#include <functional>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>

PairParticleIteratorLinkedCell::PairParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator end, ParticleContainerLinkedCell& particleContainerArg) {
    currentCellIdx = {0, 0, 0};
    particleContainer = particleContainerArg;
    currentCell = currentCell = it;

    // dont initialize the other variables if it already points to the end
    if (it == end){
        return;
    }

    // initialize iterator
    numCells = particleContainerArg.getNumCells();
    currentParticle = (*currentCell).getParticles().begin();

    // get iterator and end of iterator for neighbors of the current cell
    neighborCellsVector = getNeighborCells();
    neighborCell = neighborCellsVector.begin();
    neighborParticle = (*neighborCell).getParticles().begin();

    // move to the first valid pair
    while ((*currentParticle) == (*neighborParticle) || (neighborParticle->getPairsFlag())) {
        ++(*this);
    }
}

std::vector<Cell> PairParticleIteratorLinkedCell::getNeighborCells() {
    std::vector<Cell> neighborCells = std::vector<Cell>();
    for (int z = -1; z < 2; z++) {
        for (int y = -1; y < 2; y++) {
            for (int x = -1; x < 2; x++) {
                std::array<int, 3> relIdx = {x, y, z};
                auto neighborIdx = particleContainer.discreteCoordsToIndex(ArrayUtils::elementWiseScalarOp(currentCellIdx, relIdx, std::plus<>()));
                if (neighborIdx != -1) {
                    neighborCells.push_back(particleContainer.getCell(neighborIdx));
                }
            }
        }
    }
    return neighborCells;
}

void PairParticleIteratorLinkedCell::incrementCurrIdx() {
    if (currentCellIdx[0] == (numCells[0] - 1)) {
        if (currentCellIdx[1] == (numCells[1] - 1)) {
            currentCellIdx[2] = (currentCellIdx[2] + 1) % numCells[2];
        }
        currentCellIdx[1] = (currentCellIdx[1] + 1) % numCells[1];
    }
    currentCellIdx[0] = (currentCellIdx[0] + 1) % numCells[0];
}

PairParticleIteratorLinkedCell::reference PairParticleIteratorLinkedCell::operator*() {
    return {*currentParticle, *neighborParticle};
}


PairParticleIteratorLinkedCell& PairParticleIteratorLinkedCell::operator++() {
    neighborParticle++;
    if (neighborParticle == (*neighborCell).getParticles().end()) {
        neighborCell++;
        if (neighborCell == neighborCellsVector.end()) {
            (*currentParticle).setPairsFlag(true);
            currentParticle++;
            if (currentParticle == (*currentCell).getParticles().end()) {
                currentCell++;
                if (currentCell == particleContainer.getMesh().end()) {
                    return *this;
                }
                incrementCurrIdx();
                neighborCellsVector = getNeighborCells();
                currentParticle = (*currentCell).getParticles().begin();
            }
            neighborCell = neighborCellsVector.begin();
        }
        neighborParticle = (*neighborCell).getParticles().begin();
    }
    if ((*currentParticle) == (*neighborParticle) || (neighborParticle->getPairsFlag())) {
        return ++(*this);
    }
    return *this;
}
