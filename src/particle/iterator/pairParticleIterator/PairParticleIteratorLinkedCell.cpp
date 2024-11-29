#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include <array>
#include <cstddef>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h>

PairParticleIteratorLinkedCell::PairParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator end, std::vector<Cell>& meshArg, std::array<size_t, 3> numCellsArg) {
    completedPairs.clear();
    currentCellIdx = 0;
    mesh = meshArg;
    numCells = numCellsArg;
    currentCell = it;

    // dont initialize the other variables if it already points to the end
    if (it == end){
        return;
    }

    // initialize iterator
    currentParticle = (*currentCell).getParticles().begin();

    // get iterator and end of iterator for neighbors of the current cell
    neighborCellsVector = getNeighborCells();
    neighborCell = neighborCellsVector.begin();
    neighborParticle = (*neighborCell).getParticles().begin();

    // move to the first valid pair
    while ((*currentParticle) == (*neighborParticle) || (completedPairs.count(*neighborParticle) > 0)) {
        ++(*this);
    }
}

std::vector<Cell> PairParticleIteratorLinkedCell::getNeighborCells() {
    std::vector<Cell> neighborCells = std::vector<Cell>();
    for (int z = -1; z < 2; z++) {
        for (int y = -1; y < 2; y++) {
            for (int x = -1; x < 2; x++) {
                int neighborIdx = currentCellIdx + x + (y * numCells[0]) + (z * numCells[0] * numCells[1]);
                if (neighborIdx >= 0 && neighborIdx < mesh.size()) {
                    neighborCells.push_back(mesh[neighborIdx]);
                }
            }
        }
    }
    return neighborCells;
}

PairParticleIteratorLinkedCell::reference PairParticleIteratorLinkedCell::operator*() {
    return {*currentParticle, *neighborParticle};
}


PairParticleIteratorLinkedCell& PairParticleIteratorLinkedCell::operator++() {
    neighborParticle++;
    if (neighborParticle == (*neighborCell).getParticles().end()) {
        neighborCell++;
        if (neighborCell == neighborCellsVector.end()) {
            completedPairs.insert(*currentParticle);
            currentParticle++;
            if (currentParticle == (*currentCell).getParticles().end()) {
                currentCell++;
                if (currentCell == mesh.end()) {
                    return *this;
                }
                currentCellIdx++;
                neighborCellsVector = getNeighborCells();
                currentParticle = (*currentCell).getParticles().begin();
            }
            neighborCell = neighborCellsVector.begin();
        }
        neighborParticle = (*neighborCell).getParticles().begin();
    }
    // todo do-while instead?
    if ((*currentParticle) == (*neighborParticle) || (completedPairs.count(*neighborParticle) > 0)) {
        return ++(*this);
    }
    return *this;
}

bool PairParticleIteratorLinkedCell::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorLinkedCell*>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    } 
    return false;
}

