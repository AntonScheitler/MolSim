#include "ParticleContainerLinkedCell.h"
#include <utils/ArrayUtils.h>

ParticleContainerLinkedCell::ParticleContainerLinkedCell(std::array<double, 3> domainDimsArg, double cutoffRadiusArg) {
    this->cutoffRadius = cutoffRadiusArg;
    this->domainDims = domainDimsArg;
    initMesh();
}

void ParticleContainerLinkedCell::initMesh() {

    // initialize size for cells
    cellSize = {cutoffRadius, cutoffRadius, 1};

    // adjust cell size depending on the domain size
    for (int i = 0; i < 3; i++) {
        // the number of cells per dimension is rounded down
        // this means, the cutoffRadius will always fit in a cell, if it's center is
        // at the center of the cell
        numCells[i] = floor(domainDims[i] / cellSize[i]);
        cellSize[i] = domainDims[i] / numCells[i];
        // add halo cells
        numCells[i] += 2;
    }

    // create cells (inner, boundary, halo)
    for (int x = 0; x < numCells[0]; x++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int z = 0; z < numCells[2]; z++) {

                bool isBoundary = x == 1 || y == 1 || z == 1
                                  || x == numCells[0] - 2 || y == numCells[1] - 2 || z == numCells[2] - 2;

                bool isHalo = x == 0 || y == 0 || z == 0 || x == numCells[0] - 1 || y == numCells[1] - 1 ||
                              z == numCells[2] - 1;

                mesh.emplace_back(isBoundary, isHalo);
            }
        }
    }
}

void ParticleContainerLinkedCell::addParticle(const Particle &particle) {
    mesh[continuousCoordsToIndex(particle.getX())].addParticle(particle);
}

int ParticleContainerLinkedCell::discreteCoordsToIndex( std::array<int, 3> coord) {
    return (coord[0] * numCells[1] * numCells[2]) + (coord[1] * numCells[2]) + coord[2];
}

int ParticleContainerLinkedCell::continuousCoordsToIndex( std::array<double, 3> coord) {
    // convert continuous coords into continuous approximation of discrete coords
    std::array<double, 3> approxDiscreteCoords = ArrayUtils::elementWisePairOp(coord, cellSize, std::divides<>());
    // floor continuous approximation of discrete coords to achieve discrete coords
    return discreteCoordsToIndex(std::array<int, 3>{(int)floor(approxDiscreteCoords[0]), (int)floor(approxDiscreteCoords[1]), (int)floor(approxDiscreteCoords[2])});
}

int ParticleContainerLinkedCell::size() {
    int size = 0;
    for (auto &i: mesh) {
        size += i.size();
    }
    return size;
}

void ParticleContainerLinkedCell::correctParticleCell(Particle& p) {

    // TODO: ghost particle interaction when boundaryCondition is reflecting
    // checks particle if it should be removed because it is outflowing the grid

    int oldCellIndex = continuousCoordsToIndex(p.getOldX());
    int newCellIndex = continuousCoordsToIndex(p.getX());
    if(newCellIndex < 0 || newCellIndex >= mesh.size()) {
        SPDLOG_WARN("particle new cell index is out of bounds, index: {0}", newCellIndex);
        return;
    }
    if (oldCellIndex != newCellIndex) {
        Cell oldCell = mesh[oldCellIndex];
        Cell newCell = mesh[newCellIndex];
        // particle is not in its original cell anymore
        if (oldCell.isBoundary() && newCell.isHalo() && boundaryCondition == outflowing) {
            // remove particle completely because it is outflowing
            oldCell.removeParticle(p);
        } else if (!oldCell.isHalo() && !newCell.isHalo()) {
            // move particle to other (non-halo) cell
            oldCell.removeParticle(p);
            newCell.addParticle(p);
        }
    }
}

void ParticleContainerLinkedCell::correctAllParticlesCell() {
    for(auto c : mesh) {
        for(auto p : c.getParticles()) {
            correctParticleCell(p);
        }
    }
}

Cell &ParticleContainerLinkedCell::getCell(int index) {
    return mesh[index];
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

ParticleIterator ParticleContainerLinkedCell::begin() {
//    return ParticleIterator();
    // TODO
}

ParticleIterator ParticleContainerLinkedCell::end() {
//    return ParticleIterator(__gnu_cxx::__normal_iterator());
    // TODO
}

PairParticleIterator ParticleContainerLinkedCell::beginPairParticle() {
    // TODO
}

PairParticleIterator ParticleContainerLinkedCell::endPairParticle() {
    // TODO
}

