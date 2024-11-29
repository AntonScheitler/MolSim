#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "utils/ArrayUtils.h"
#include <cstdlib>
#include <functional>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>

PairParticleIteratorBoundaryNHalo::PairParticleIteratorBoundaryNHalo(std::vector<Cell>::iterator it, std::vector<Cell> meshArg, std::array<size_t, 3> numCellsArg, std::array<double, 3> cellSizeArg) {
    mesh = meshArg;
    currentCellIdx = {0, 0, 0};
    numCells = numCellsArg;
    cellSize = cellSizeArg;

    currentCell = it;
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

std::vector<Particle> PairParticleIteratorBoundaryNHalo::createGhostParticles(Particle& particle) {
    std::vector<Particle> ghosts = {};
    // add ghost particle to the "left" of the particle
    if (currentCellIdx[0] == 0) {
        double deltaX = std::abs(currentParticle->getX()[0]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0] - deltaX, currentParticle->getX()[1], currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({-1., 1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    } 
    // add ghost particle to the "right" of the particle
    if (currentCellIdx[0] == numCells[0] - 1) {
        double deltaX = std::abs((numCells[0] * cellSize[0]) - currentParticle->getX()[0]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0] + deltaX, currentParticle->getX()[1], currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({-1., 1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle "below" the particle
    if (currentCellIdx[1] == 0) {
        double deltaY = std::abs(currentParticle->getX()[1]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[1], currentParticle->getX()[1] - deltaY, currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., -1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle "above" the particle
    if (currentCellIdx[1] == numCells[1] - 1) {
        double deltaY = std::abs((numCells[1] * cellSize[1]) - currentParticle->getX()[1]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[1], currentParticle->getX()[1] + deltaY, currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., -1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle to the "bottom" of the particle
    if (currentCellIdx[2] == 0) {
        double deltaZ = std::abs(currentParticle->getX()[2]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[1], currentParticle->getX()[1], currentParticle->getX()[2] - deltaZ};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., 1., -1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle to the "top" of the particle
    if (currentCellIdx[2] == 0) {
        double deltaZ = std::abs((numCells[2] * cellSize[2]) - currentParticle->getX()[2]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[1], currentParticle->getX()[1], currentParticle->getX()[2] + deltaZ};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., 1., -1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    return ghosts;
}

PairParticleIteratorBoundaryNHalo::reference  PairParticleIteratorBoundaryNHalo::operator*() {
    return {*currentParticle, *currentGhost};
}

PairParticleIteratorBoundaryNHalo& PairParticleIteratorBoundaryNHalo::operator++() {
    currentGhost++;
    if (currentGhost == currentGhostEnd) {
        currentParticle++;
        if (currentParticle == currentCell->getParticles().end()) {
            do {
                incrementIdx3D(currentCellIdx);
                currentCell++;
                if (currentCell == mesh.end()) {
                    return *this;
                }
            } while (!(currentCell->isBoundary));
            currentParticle = currentCell->getParticles().begin();
        }
        auto ghosts = createGhostParticles(*currentParticle);
        currentGhost = ghosts.begin();
        currentGhostEnd = ghosts.end();
    }
    return *this; 
}

bool PairParticleIteratorBoundaryNHalo::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorBoundaryNHalo*>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    } 
    return false;
}
