#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "utils/ArrayUtils.h"
#include <cstdlib>
#include <functional>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>

    PairParticleIteratorBoundaryNHalo::PairParticleIteratorBoundaryNHalo(std::vector<Cell>::iterator currentCellArg, std::vector<Cell>::iterator currentCellEndArg, std::vector<Cell> meshArg, std::array<size_t, 3> numCellsArg,
            std::array<double, 3> cellSizeArg, struct boundaryConfig boundaryConfigArg){
    mesh = meshArg;
    currentCellIdx = {0, 0, 0};
    numCells = numCellsArg;
    cellSize = cellSizeArg;
    boundaryConfig = boundaryConfigArg;
    currentCell = currentCellArg;
    currentCellEnd = currentCellEndArg;
    stepToNonEmptyBoundaryCell(false);
}

void PairParticleIteratorBoundaryNHalo::incrementCurrentCellIdx() {
    if (currentCellIdx[0] == numCells[0] - 1) {
        if (currentCellIdx[1] == numCells[1] - 1) {
            currentCellIdx[2] = (currentCellIdx[2] + 1) % numCells[2];
        }
        currentCellIdx[1] = (currentCellIdx[1] + 1) % numCells[1];
    }
    currentCellIdx[0] = (currentCellIdx[0] + 1) % numCells[0];
}

void  PairParticleIteratorBoundaryNHalo::stepToNonEmptyBoundaryCell(bool stepToNext) {
    do {
        if (stepToNext){
            ++currentCell;
            incrementCurrentCellIdx();
        }
        stepToNext = true;
        while (currentCell != currentCellEnd && (!currentCell->isBoundary || currentCell->getParticles().empty())) {
            ++currentCell;
            incrementCurrentCellIdx();
        }

        if (currentCell == currentCellEnd) {
            return;
        }
        currentParticle = currentCell->getParticles().begin();
        ghostsVector = createGhostParticles();
        currentGhost = ghostsVector.begin();
        currentGhostEnd = ghostsVector.end();
    } while (currentGhost == currentGhostEnd);
}

std::vector<Particle> PairParticleIteratorBoundaryNHalo::createGhostParticles() {
    std::vector<Particle> ghosts = std::vector<Particle>();
    // add ghost particle to the "left" of the particle
    if (currentCellIdx[0] == 0 && boundaryConfig.x[0] == ghost) {
        double deltaX = std::abs(currentParticle->getX()[0]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0] - deltaX, currentParticle->getX()[1], currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({-1., 1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    } 
    // add ghost particle to the "right" of the particle
    if (currentCellIdx[0] == numCells[0] - 1 && boundaryConfig.x[1] == ghost) {
        double deltaX = std::abs((numCells[0] * cellSize[0]) - currentParticle->getX()[0]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0] + deltaX, currentParticle->getX()[1], currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({-1., 1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle "below" the particle
    if (currentCellIdx[1] == 0 && boundaryConfig.y[0] == ghost) {
        double deltaY = std::abs(currentParticle->getX()[1]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0], currentParticle->getX()[1] - deltaY, currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., -1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle "above" the particle
    if (currentCellIdx[1] == numCells[1] - 1 && boundaryConfig.y[1] == ghost) {
        double deltaY = std::abs((numCells[1] * cellSize[1]) - currentParticle->getX()[1]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0], currentParticle->getX()[1] + deltaY, currentParticle->getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., -1., 1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle to the "bottom" of the particle
    if (currentCellIdx[2] == 0 && boundaryConfig.z[0] == ghost) {
        double deltaZ = std::abs(currentParticle->getX()[2]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0], currentParticle->getX()[1], currentParticle->getX()[2] - deltaZ};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp({1., 1., -1.}, currentParticle->getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle->getM());
        ghosts.push_back(ghost);
    }
    // add ghost particle to the "top" of the particle
    if (currentCellIdx[2] == 0 && boundaryConfig.z[1] == ghost) {
        double deltaZ = std::abs((numCells[2] * cellSize[2]) - currentParticle->getX()[2]) * 2;
        std::array<double, 3> ghostPos = {currentParticle->getX()[0], currentParticle->getX()[1], currentParticle->getX()[2] + deltaZ};
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
            stepToNonEmptyBoundaryCell(true);
            return *this;
        }
        ghostsVector = createGhostParticles();
        currentGhost = ghostsVector.begin();
        currentGhostEnd = ghostsVector.end();
    }
    return *this; 
}

bool PairParticleIteratorBoundaryNHalo::operator!=(const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorBoundaryNHalo*>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    } 
    return true;
}
