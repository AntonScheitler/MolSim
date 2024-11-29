#include <algorithm>
#include <particle/container/ParticleContainerLinkedCell.h>
#include "particle/iterator/particleIterator/ParticleIteratorLinkedCell.h"
#include <utils/ArrayUtils.h>
#include "spdlogConfig.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(std::array<double, 3> domainDimsArg, double cutoffRadiusArg) {
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
    if (coord[0] < 0 || coord[1] < 0 || coord[2] < 2) return -1;
    if (coord[0] >= numCells[0] || coord[1] >= numCells[1] || coord[2] >= numCells[2]) return -1;

    return coord[0]  + (coord[1] * numCells[0]) + (coord[2] * numCells[0] * numCells[1]);
}

int ParticleContainerLinkedCell::continuousCoordsToIndex( std::array<double, 3> coord) {
    // convert continuous coords into continuous approximation of discrete coords
    std::array<double, 3> approxDiscreteCoords = ArrayUtils::elementWisePairOp(coord, cellSize, std::divides<>());
    // floor continuous approximation of discrete coords to achieve discrete coords
    return discreteCoordsToIndex(std::array<int, 3>{(int)floor(approxDiscreteCoords[0]), (int)floor(approxDiscreteCoords[1]), (int)floor(approxDiscreteCoords[2])});
}

ParticleIteratorLinkedCell ParticleContainerLinkedCell::begin() {
    return ParticleIteratorLinkedCell(mesh.begin(), mesh.end());
}

ParticleIteratorLinkedCell ParticleContainerLinkedCell::end() {
    return ParticleIteratorLinkedCell(mesh.end(), mesh.end());
}

std::vector<Cell>& ParticleContainerLinkedCell::getMesh() {
    return mesh;
}


Cell& ParticleContainerLinkedCell::getCell(int idx) {
    return mesh[idx];
}

PairParticleIteratorLinkedCell ParticleContainerLinkedCell::beginPairs() {
    return {mesh.begin(), mesh.end(), mesh, numCells};
}

PairParticleIteratorLinkedCell ParticleContainerLinkedCell::endPairs() {
    return {mesh.end(), mesh.end(), mesh, numCells};
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::beginGhosts() {
    return {mesh.begin(), mesh, numCells, cellSize};
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::endGhosts() {
    return {mesh.end(), mesh, numCells, cellSize};
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
    if(newCellIndex < 0 || newCellIndex >= mesh.size()) {
        SPDLOG_WARN("particle new cell index is out of bounds, index: {0}", newCellIndex);
        return;
    }
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

double ParticleContainerLinkedCell::getAverageVelocity() {
    return averageVelocity;
}

void ParticleContainerLinkedCell::setAverageVelocity(double averageVelocityArg) {
    this->averageVelocity = averageVelocityArg;
}

Particle &ParticleContainerLinkedCell::getParticle(int index) {
    // TODO
}
