#include <particle/container/ParticleContainerLinkedCell.h>
#include "particle/iterator/pairParticleIterator/PairParticleIteratorLinkedCell.h"
#include "particle/iterator/particleIterator/ParticleIteratorLinkedCell.h"
#include <utils/ArrayUtils.h>
#include "spdlogConfig.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(std::array<double, 3> domainSizeArg, double cutoffRadiusArg, 
        struct boundaryConfig boundaryConfigArg) {
    domainSize = domainSizeArg;
    cutoffRadius = cutoffRadiusArg;
    boundaryConfig = boundaryConfigArg;
    // initialize size for cells
    cellSize = {cutoffRadius, cutoffRadius, 1};

    // adjust cell size depending on the domain size
    for (int i = 0; i < 3; i++) {
        // the number of cells per dimension is rounded down
        // this means, the cutoffRadius will always fit in a cell, if it'c center is
        // at the center of the cell
        // there should be at least once cell per dimension
        numCells[i] = std::max((int) floor(domainSize[i] / cellSize[i]), 1);
        cellSize[i] = domainSize[i] / (1.0 * numCells[i]);
    }

    // add all cells to the mesh
    for (int z = 0; z < numCells[0]; z++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int x = 0; x < numCells[2]; x++) {
                bool isBoundary = x == 0 || y == 0 || z == 0 || x == numCells[0] - 1 ||
                    y == numCells[1] - 1 || z == numCells[2] - 1;
                Cell cell = Cell(isBoundary);
                mesh.push_back(cell);
            }
        }
    }
}

void ParticleContainerLinkedCell::addParticle(const Particle &particle) {
    mesh[continuousCoordsToIndex(particle.getX())].addParticle(particle);
}

int ParticleContainerLinkedCell::discreteCoordsToIndex(std::array<int, 3> coord) {
    // check if coord is out of bounds and return -1 if it is
    if (coord[0] < 0 || coord[1] < 0 || coord[2] < 0) return -1;
    if (coord[0] >= numCells[0] || coord[1] >= numCells[1] || coord[2] >= numCells[2]) return -1;

    return coord[0]  + (coord[1] * numCells[0]) + (coord[2] * numCells[0] * numCells[1]);
}

int ParticleContainerLinkedCell::continuousCoordsToIndex( std::array<double, 3> coord) {
    // convert continuous coords into continuous approximation of discrete coords
    std::array<double, 3> approxDiscreteCoords = ArrayUtils::elementWisePairOp(coord, cellSize, std::divides<>());
    // floor continuous approximation of discrete coords to achieve discrete coords
    return discreteCoordsToIndex(std::array<int, 3>{(int)floor(approxDiscreteCoords[0]), (int)floor(approxDiscreteCoords[1]), (int)floor(approxDiscreteCoords[2])});
}

std::vector<Cell>& ParticleContainerLinkedCell::getMesh() {
    return mesh;
}


Cell& ParticleContainerLinkedCell::getCell(int idx) {
    return mesh[idx];
}

std::unique_ptr<ParticleIterator> ParticleContainerLinkedCell::begin() {
    return std::unique_ptr<ParticleIterator>(new ParticleIteratorLinkedCell(mesh.begin(), mesh.end()));
}

std::unique_ptr<ParticleIterator> ParticleContainerLinkedCell::end() {
    return std::unique_ptr<ParticleIterator>(new ParticleIteratorLinkedCell(mesh.end(), mesh.end()));
}

std::unique_ptr<PairParticleIterator> ParticleContainerLinkedCell::beginPairParticle() {
    return std::unique_ptr<PairParticleIterator>(new PairParticleIteratorLinkedCell(mesh.begin(), mesh.end(), mesh, numCells));
}

std::unique_ptr<PairParticleIterator> ParticleContainerLinkedCell::endPairParticle() {
    return std::unique_ptr<PairParticleIterator>(new PairParticleIteratorLinkedCell(mesh.end(), mesh.end(), mesh, numCells));
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::beginPairGhost() {
    return {mesh.begin(), mesh.end(), mesh, numCells, cellSize, boundaryConfig};
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::endPairGhost() {
    return {mesh.end(), mesh.end(), mesh, numCells, cellSize, boundaryConfig};
}

int ParticleContainerLinkedCell::size() {
    int size = 0;
    for (auto& i: mesh) {
        size += i.getParticles().size();
    }
    return size;
}

void ParticleContainerLinkedCell::correctParticleIndex(Particle& p) {

    // TODO: ghost particle interaction when boundaryCondition is reflecting
    // checks particle if it should be removed because it is outflowing the grid

    int oldCellIndex = continuousCoordsToIndex(p.getOldX());
    int newCellIndex = continuousCoordsToIndex(p.getX());
    // particle is not in its original cell anymore
    if (oldCellIndex != newCellIndex) {
        Cell oldCell = getCell(oldCellIndex);
        oldCell.removeParticle(p);
        // check if the cell is still within bounds
        if (newCellIndex != -1) {
            Cell newCell = getCell(newCellIndex);
            newCell.addParticle(p);
        }
    }
}

void ParticleContainerLinkedCell::correctAllParticleIndecies() {
    for(auto& c : mesh) {
        for(auto& p : c.getParticles()) {
            correctParticleIndex(p);
        }
    }
}


std::array<size_t, 3> ParticleContainerLinkedCell::getNumCells() {
    return numCells;
}

std::array<double, 3> ParticleContainerLinkedCell::getCellSize() {
    return cellSize;
}

double ParticleContainerLinkedCell::getAverageVelocity() {
    return 0.0;
}

void ParticleContainerLinkedCell::setAverageVelocity(double in) {

}

Particle& ParticleContainerLinkedCell::getParticle(int index) {
    return mesh[0].getParticles()[0];
}
