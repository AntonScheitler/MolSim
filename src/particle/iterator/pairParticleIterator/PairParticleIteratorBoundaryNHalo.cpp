#include "particle/iterator/pairParticleIterator/PairParticleIterator.h"
#include "utils/ArrayUtils.h"
#include <cstdlib>
#include <functional>
#include <memory>
#include <particle/iterator/pairParticleIterator/PairParticleIteratorBoundaryNHalo.h>

PairParticleIteratorBoundaryNHalo::PairParticleIteratorBoundaryNHalo( std::vector<Cell>::iterator currentCellArg, std::vector<Cell>::iterator currentCellEndArg, 
        std::vector<Cell> &meshArg, std::array<size_t, 3> numCellsArg, std::array<double, 3> cellSizeArg, struct boundaryConfig boundaryConfigArg,
            std::vector<Particle> &particlesArg) : mesh(meshArg), particles(particlesArg) {
    currentCellIdx = {0, 0, 0};
    numCells = numCellsArg;
    cellSize = cellSizeArg;
    boundaryConfig = boundaryConfigArg;
    currentCell = currentCellArg;
    currentCellEnd = currentCellEndArg;
    // skip forward to the first cell which needs ghosts
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

void PairParticleIteratorBoundaryNHalo::stepToNonEmptyBoundaryCell(bool stepToNext) {
    do {
        if (stepToNext) {
            ++currentCell;
            incrementCurrentCellIdx();
        }
        stepToNext = true;
        // search for a non-empty boundary cell
        while (currentCell != currentCellEnd && (!currentCell->isBoundary || currentCell->getParticlesIndices().empty())) {
            ++currentCell;
            incrementCurrentCellIdx();
        }

        // if there are no non-empty boundary cells, return
        if (currentCell == currentCellEnd) {
            return;
        }
        // if one has been found, currentParticle is updated
        currentParticleIdx = currentCell->getParticlesIndices().begin();
        // the ghost particles for the boundary particle need to be computed
        updateGhostsVector();
        currentGhost = ghostsVector.begin();
        currentGhostEnd = ghostsVector.end();
        // if there are no ghost particles for the boundary particle (because the
        // boundary is outflowing, etc.), repeat
    } while (currentGhost == currentGhostEnd);
}

void PairParticleIteratorBoundaryNHalo::updateGhostsVector() {
    ghostsVector.clear();
    Particle &currentParticle = particles[*currentParticleIdx];
    // add ghost particle to the "left" of the particle
    if (currentCellIdx[0] == 0 && boundaryConfig.x[0] == reflect) {
        double deltaX = std::abs(currentParticle.getX()[0]) * 2;
        std::array<double, 3> ghostPos = {currentParticle.getX()[0] - deltaX,
            currentParticle.getX()[1],
            currentParticle.getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp(
                {-1., 1., 1.}, currentParticle.getV(), std::multiplies<>());
        ghostsVector.push_back(std::make_unique<Particle>(Particle(ghostPos, ghostVel, currentParticle.getM())));
    }
    // add ghost particle to the "right" of the particle
    if (currentCellIdx[0] == numCells[0] - 1 && boundaryConfig.x[1] == reflect) {
        double deltaX =
            std::abs((numCells[0] * cellSize[0]) - currentParticle.getX()[0]) * 2;
        std::array<double, 3> ghostPos = {currentParticle.getX()[0] + deltaX,
            currentParticle.getX()[1],
            currentParticle.getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp(
                {-1., 1., 1.}, currentParticle.getV(), std::multiplies<>());
        ghostsVector.push_back(std::make_unique<Particle>(Particle(ghostPos, ghostVel, currentParticle.getM())));
    }
    // add ghost particle "below" the particle
    if (currentCellIdx[1] == 0 && boundaryConfig.y[0] == reflect) {
        double deltaY = std::abs(currentParticle.getX()[1]) * 2;
        std::array<double, 3> ghostPos = {currentParticle.getX()[0],
            currentParticle.getX()[1] - deltaY,
            currentParticle.getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp(
                {1., -1., 1.}, currentParticle.getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle.getM());
        ghostsVector.push_back(std::make_unique<Particle>(Particle(ghostPos, ghostVel, currentParticle.getM())));
    }
    // add ghost particle "above" the particle
    if (currentCellIdx[1] == numCells[1] - 1 && boundaryConfig.y[1] == reflect) {
        double deltaY =
            std::abs((numCells[1] * cellSize[1]) - currentParticle.getX()[1]) * 2;
        std::array<double, 3> ghostPos = {currentParticle.getX()[0],
            currentParticle.getX()[1] + deltaY,
            currentParticle.getX()[2]};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp(
                {1., -1., 1.}, currentParticle.getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle.getM());
        ghostsVector.push_back(std::make_unique<Particle>(Particle(ghostPos, ghostVel, currentParticle.getM())));
    }
    // add ghost particle to the "bottom" of the particle
    if (currentCellIdx[2] == 0 && boundaryConfig.z[0] == reflect) {
        double deltaZ = std::abs(currentParticle.getX()[2]) * 2;
        std::array<double, 3> ghostPos = {currentParticle.getX()[0],
            currentParticle.getX()[1],
            currentParticle.getX()[2] - deltaZ};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp(
                {1., 1., -1.}, currentParticle.getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle.getM());
        ghostsVector.push_back(std::make_unique<Particle>(Particle(ghostPos, ghostVel, currentParticle.getM())));
    }
    // add ghost particle to the "top" of the particle
    if (currentCellIdx[2] == 0 && boundaryConfig.z[1] == reflect) {
        double deltaZ =
            std::abs((numCells[2] * cellSize[2]) - currentParticle.getX()[2]) * 2;
        std::array<double, 3> ghostPos = {currentParticle.getX()[0],
            currentParticle.getX()[1],
            currentParticle.getX()[2] + deltaZ};
        std::array<double, 3> ghostVel = ArrayUtils::elementWisePairOp(
                {1., 1., -1.}, currentParticle.getV(), std::multiplies<>());
        Particle ghost = Particle(ghostPos, ghostVel, currentParticle.getM());
        ghostsVector.push_back(std::make_unique<Particle>(Particle(ghostPos, ghostVel, currentParticle.getM())));
    }
}

PairParticleIteratorBoundaryNHalo::reference
PairParticleIteratorBoundaryNHalo::operator*() {
    return {particles[*currentParticleIdx], **currentGhost};
}

PairParticleIteratorBoundaryNHalo &
PairParticleIteratorBoundaryNHalo::operator++() {
    // get the next ghost particle
    currentGhost++;
    if (currentGhost == currentGhostEnd) {
        // move to the next particle if there are no more ghosts
        currentParticleIdx++;
        if (currentParticleIdx == currentCell->getParticlesIndices().end()) {
            // search for the next boundary cell which needs ghosts
            stepToNonEmptyBoundaryCell(true);
            return *this;
        }
        updateGhostsVector();
        currentGhost = ghostsVector.begin();
        currentGhostEnd = ghostsVector.end();
    }
    return *this;
}

bool PairParticleIteratorBoundaryNHalo::operator!=(
        const PairParticleIterator &other) {
    auto casted = dynamic_cast<const PairParticleIteratorBoundaryNHalo *>(&other);
    if (casted) {
        // this has a high false-positive rate because this is sufficient to
        // distinguish the iterator from the end
        return currentCell != casted->currentCell;
    }
    return true;
}
