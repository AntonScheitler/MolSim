#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>

PairParticleIteratorBoundaryNHalo::PairParticleIteratorBoundaryNHalo( std::vector<Cell> meshArg, std::array<size_t, 3> numCellsArg, std::array<double, 3> cellSizeArg) {
    mesh = meshArg;
    currentCellIdx = {0, 0, 0};
    numCells = numCellsArg;
    cellSize = cellSizeArg;

    currentCell = mesh.begin();
    if (currentCell == mesh.end()) {
        return;
    }
    currentParticle = currentCell->getParticles().begin();
    auto ghosts = createGhostParticles(*currentParticle);
    currentGhost = ghosts.begin();
    currentGhostEnd = ghosts.end();
}

void PairParticleIteratorBoundaryNHalo::incrementIdx3D(std::array<int, 3>& idx) {
    if (idx[0] == numCells[0] - 1) {
        if (idx[1] == numCells[1] - 1) {
            idx[2] = (idx[2] + 1) % numCells[2];
        }
        idx[1] = (idx[1] + 1) % numCells[1];
    }
    idx[0] = (idx[0] + 1) % numCells[0];
}


PairParticleIteratorBoundaryNHalo::reference  PairParticleIteratorBoundaryNHalo::operator*() {
    return {*currentParticle, *currentGhost};
}

PairParticleIteratorBoundaryNHalo& PairParticleIteratorBoundaryNHalo::operator++() {
    currentGhost++;
    if (currentGhost == currentGhostEnd) {
        currentParticle++;
        if (currentParticle == currentCell->getParticles().end()) {
            currentCell++;
            if (currentCell == mesh.end()) {
                return *this;
            }
            currentParticle = currentCell->getParticles().begin();
        }
        auto ghosts = createGhostParticles(*currentParticle);
        currentGhost = ghosts.begin();
        currentGhostEnd = ghosts.end();
    }
    return *this; 
}

bool PairParticleIteratorBoundaryNHalo::operator!=(const PairParticleIteratorBoundaryNHalo &other) {
    return currentCell != other.currentCell;
}


