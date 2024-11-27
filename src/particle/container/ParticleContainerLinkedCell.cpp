#include <particle/container/ParticleContainerLinkedCell.h>
#include "particle/iterator/particleIterator/ParticleIteratorLinkedCell.h"
#include <utils/ArrayUtils.h>

ParticleContainerLinkedCell::ParticleContainerLinkedCell(
        const std::vector<Particle> &particlesArg, std::vector<double> domainSize,
        double cutoffRadius) {
}

// TODO move to constructor?
void ParticleContainerLinkedCell::initMesh(std::array<double, 3> domainSize,
        double cutoffRadius) {
    // initialize size for cells
    cellSize = {cutoffRadius, cutoffRadius, 1};

    // adjust cell size depending on the domain size
    for (int i = 0; i < 3; i++) {
        // the number of cells per dimension is rounded down
        // this means, the cutoffRadius will always fit in a cell, if it'c center is
        // at the center of the cell
        numCells[i] = floor(domainSize[i] / cellSize[i]);
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

int ParticleContainerLinkedCell::discreteCoordsToIndex( std::array<int, 3> coord) {
    // check if coord is out of bounds and return -1 if it is
    if (coord[0] < 0 || coord[1] < 0 || coord[2] < 2) return -1;
    if (coord[0] >= numCells[0] || coord[1] >= numCells[1] || coord[2] >= numCells[2]) return -1;

    return coord[0]  + (coord[1] * numCells[0]) + (coord[2] * numCells[0] * numCells[1]);
}

int ParticleContainerLinkedCell::continuousCoordsToIndex( std::array<double, 3> coord) {
    // convert continuous coords into continuous approximation of discrete coords
    std::array<double, 3> approxDiscreteCoords = ArrayUtils::elementWisePairOp(coord, cellSize, std::divides<>());
    // floor continuous approximation of discrete coords to achieve discrete coords
    return discreteCoordsToIndex(std::array<int, 3>{ (int)floor(approxDiscreteCoords[0]), (int)floor(approxDiscreteCoords[1]), (int)floor(approxDiscreteCoords[2])});
}

ParticleIteratorLinkedCell ParticleContainerLinkedCell::begin() {
    return ParticleIteratorLinkedCell(mesh.begin());
}

ParticleIteratorLinkedCell ParticleContainerLinkedCell::end() {
    return ParticleIteratorLinkedCell(mesh.end());
}

std::vector<Cell>& ParticleContainerLinkedCell::getMesh() {
    return mesh;
}


Cell& ParticleContainerLinkedCell::getCell(int idx) {
    return mesh[idx];
}
