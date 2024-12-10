#include <cstdio>
#include <memory>
#include <particle/container/ParticleContainerLinkedCell.h>
#include "particle/Particle.h"
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
        // this means, the dimensions of a cell will always be greater or equal to the cutoff radius, unless the domain
        // is smaller than the cutoff radius of course
        // there should be at least once cell per dimension
        numCells[i] = std::max(static_cast<int>(floor(domainSize[i] / cellSize[i])), 1);
        cellSize[i] = domainSize[i] / numCells[i];
    }

    // add all cells to the mesh
    size_t cellId = 0;
    for (int z = 0; z < numCells[0]; z++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int x = 0; x < numCells[2]; x++) {
                bool isBoundary = x == 0 || y == 0 || z == 0 || x == numCells[0] - 1 ||
                                  y == numCells[1] - 1 || z == numCells[2] - 1;
                Cell cell = Cell(isBoundary, cellId);
                mesh.push_back(cell);
                cellId++;
            }
        }
    }
}

void ParticleContainerLinkedCell::addParticle(const Particle &particle) {
    mesh[continuousCoordsToIndex(particle.getX())].addParticle(particle);
}

std::array<double, 3> ParticleContainerLinkedCell::applyPeriodicBoundaries(std::array<double, 3> coord) {
    double offsetX = numCells[0] * cellSize[0];
    double offsetY = numCells[1] * cellSize[1];
    double offsetZ = numCells[2] * cellSize[2];

    if (boundaryConfig.x[0] == periodic || boundaryConfig.x[1] == periodic) {
        coord[0] = coord[0] < 0 ? coord[0] + offsetX
                    : coord[0] > offsetX ? coord[0] - offsetX
                        : coord[0];
    }
    if (boundaryConfig.y[0] == periodic || boundaryConfig.y[1] == periodic) {
        coord[1] = coord[1] < 0 ? coord[1] + offsetY
                    : coord[1] > offsetY ? coord[1] - offsetY
                        : coord[1];
    }
    if (boundaryConfig.z[0] == periodic || boundaryConfig.z[1] == periodic) {
        coord[2] = coord[2] < 0 ? coord[2] + offsetZ
                    : coord[2] > offsetZ ? coord[2] - offsetZ
                        : coord[2];
    }

    return coord;
}

int ParticleContainerLinkedCell::discreteCoordsToIndex(std::array<int, 3> coord) {
    // check if coord is out of bounds and return -1 if it is
    if ((coord[0] < 0 && boundaryConfig.x[0] != periodic) || 
            (coord[1] < 0 && boundaryConfig.y[0] != periodic) || 
                (coord[2] < 0 && boundaryConfig.z[0] != periodic)) return -1;

    if ((coord[0] >= numCells[0] && boundaryConfig.x[1] != periodic) || 
            (coord[1] >= numCells[1] && boundaryConfig.y[1] != periodic) || 
                (coord[2] >= numCells[2] && boundaryConfig.z[1] != periodic)) return -1;

    // if coord is not out of bounds, use periodic behavior
    double newX = (coord[0] + numCells[0]) % numCells[0];
    double newY = (coord[1] + numCells[1]) % numCells[1];
    double newZ = (coord[2] + numCells[2]) % numCells[2];

    return newX + (newY * numCells[0]) + (newZ * numCells[0] * numCells[1]);
}

int ParticleContainerLinkedCell::continuousCoordsToIndex(std::array<double, 3> coord) {
    int coordX = floor(coord[0] / cellSize[0]);
    int coordY = floor(coord[1] / cellSize[1]);
    int coordZ = floor(coord[2] / cellSize[2]);
    // floor continuous approximation of discrete coords to achieve discrete coords
    return discreteCoordsToIndex({coordX, coordY, coordZ});
}

bool ParticleContainerLinkedCell::correctParticleIndex(Particle &p) {
    int oldCellIndex = continuousCoordsToIndex(p.getOldX());
    int newCellIndex = continuousCoordsToIndex(p.getX());
    // particle is not in its original cell anymore
    if (oldCellIndex != newCellIndex) {

        SPDLOG_DEBUG("removing particle from cell {0}", oldCellIndex);
        // check if the cell is still within bounds
        if (newCellIndex != -1) {
            if (mesh[oldCellIndex].isBoundary) {
                p.setX(applyPeriodicBoundaries(p.getX()));
            }
            addParticle(p);
            SPDLOG_DEBUG("adding particle into cell {0}", newCellIndex);
        } else
            SPDLOG_DEBUG("particle outflowing");
        return true;
    }
    return false;
}

void ParticleContainerLinkedCell::correctAllParticleIndices() {
    for (auto &c: mesh) {
        std::vector<size_t> particlesToRemove = {};
        for (Particle& p: c.getParticles()) {
            if (correctParticleIndex(p)) {
                particlesToRemove.push_back(p.getId());
            }
        }
        for (size_t id: particlesToRemove) {
            c.removeParticle(id);
        }
    }
}

std::vector<Cell> &ParticleContainerLinkedCell::getMesh() {
    return mesh;
}


Cell &ParticleContainerLinkedCell::getCell(int idx) {
    return mesh[idx];
}

std::unique_ptr<ParticleIterator> ParticleContainerLinkedCell::begin() {
    return std::make_unique<ParticleIteratorLinkedCell>(ParticleIteratorLinkedCell(mesh.begin(), mesh.end()));
}

std::unique_ptr<ParticleIterator> ParticleContainerLinkedCell::end() {
    return std::make_unique<ParticleIteratorLinkedCell>(ParticleIteratorLinkedCell(mesh.end(), mesh.end()));
}

std::unique_ptr<PairParticleIterator> ParticleContainerLinkedCell::beginPairParticle() {
    return std::make_unique<PairParticleIteratorLinkedCell>(
            PairParticleIteratorLinkedCell(mesh.begin(), mesh.end(), mesh, numCells, boundaryConfig));
}

std::unique_ptr<PairParticleIterator> ParticleContainerLinkedCell::endPairParticle() {
    return std::make_unique<PairParticleIteratorLinkedCell>(
            PairParticleIteratorLinkedCell(mesh.end(), mesh.end(), mesh, numCells, boundaryConfig));
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::beginPairGhost() {
    return {mesh.begin(), mesh.end(), mesh, numCells, cellSize, boundaryConfig};
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::endPairGhost() {
    return {mesh.end(), mesh.end(), mesh, numCells, cellSize, boundaryConfig};
}

int ParticleContainerLinkedCell::size() {
    int size = 0;
    for (auto &i: mesh) {
        size += i.getParticles().size();
    }
    return size;
}

std::unique_ptr<ParticleContainer> ParticleContainerLinkedCell::copy() {
    return std::make_unique<ParticleContainerLinkedCell>(*this);
}


std::array<size_t, 3> ParticleContainerLinkedCell::getNumCells() {
    return numCells;
}

std::array<double, 3> ParticleContainerLinkedCell::getCellSize() {
    return cellSize;
}

double ParticleContainerLinkedCell::getCutoffRadius() {
    return cutoffRadius;
}
