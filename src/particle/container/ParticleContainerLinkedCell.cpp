#include <cmath>
#include <memory>
#include <particle/container/ParticleContainerLinkedCell.h>
#include "particle/Particle.h"
#include "particle/boundary/Boundary.h"
#include <utils/ArrayUtils.h>
#include "particle/iterator/particleIterator/ParticleIterator.h"
#include "particle/iterator/particleIterator/ParticleIteratorNonFixedParticles.h"
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
    for (int z = 0; z < numCells[2]; z++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int x = 0; x < numCells[0]; x++) {
                bool isBoundary = x == 0 || y == 0 || z == 0 || x == numCells[0] - 1 ||
                                  y == numCells[1] - 1 || z == numCells[2] - 1;
                Cell cell = Cell(isBoundary, cellId);
                mesh.push_back(cell);
                cellId++;
            }
        }
    }
    computeNeighborCellsMatrix();
}

void ParticleContainerLinkedCell::addParticle(const Particle &particle) {
    particles.push_back(particle);
    mesh[continuousCoordsToIndex(particle.getX())].addParticleIdx(particles.size() - 1);
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
        (coord[2] < 0 && boundaryConfig.z[0] != periodic))
        return -1;

    if ((coord[0] >= numCells[0] && boundaryConfig.x[1] != periodic) ||
        (coord[1] >= numCells[1] && boundaryConfig.y[1] != periodic) ||
        (coord[2] >= numCells[2] && boundaryConfig.z[1] != periodic))
        return -1;

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
    int res = discreteCoordsToIndex({coordX, coordY, coordZ});
    return res;
}

bool ParticleContainerLinkedCell::correctCellMembershipSingleParticle(size_t particleIdx) {
    Particle &particle = particles[particleIdx];
    int oldCellIndex = continuousCoordsToIndex(particle.getOldX());
    int newCellIndex = continuousCoordsToIndex(particle.getX());
    // particle is not in its original cell anymore
    if (oldCellIndex != newCellIndex) {

        SPDLOG_DEBUG("removing particle from cell {0}", oldCellIndex);
        // check if the cell is still within bounds
        if (newCellIndex != -1) {
            if (mesh[oldCellIndex].isBoundary) {
                particle.setX(applyPeriodicBoundaries(particle.getX()));
            }
            mesh[newCellIndex].addParticleIdx(particleIdx);
            particle.setOldX(particle.getX());
            SPDLOG_DEBUG("adding particle into cell {0}", newCellIndex);
        } else {
            particle.setActive(false);
            SPDLOG_DEBUG("particle outflowing");
        }
        return true;
    }
    return false;
}

void ParticleContainerLinkedCell::correctCellMembershipAllParticles() {
    for (Cell &cell: mesh) {
        std::vector<size_t> particleIndicesToRemove = {};
        for (size_t particleIdx: cell.getParticlesIndices()) {
            if (correctCellMembershipSingleParticle(particleIdx)) {
                particleIndicesToRemove.push_back(particleIdx);
            }
        }
        for (size_t idx: particleIndicesToRemove) {
            cell.removeParticle(idx);
        }
    }
}

void ParticleContainerLinkedCell::getPeriodicDistanceVector(const std::array<double, 3> &point1,
                                                            const std::array<double, 3> &point2,
                                                            std::array<double, 3> &v) {
    // iterate through every axis
    for (int axis = 0; axis < 3; axis++) {
        // check if point1 is on one side of the boundary of the given axis and point2 is on the other 
        if ((axis == 0 && boundaryConfig.x[0] == periodic) || (axis == 1 && boundaryConfig.y[0] == periodic) ||
            (axis == 2 && boundaryConfig.z[0] == periodic)) {
            if ((point1[axis] < 0 + cellSize[axis] &&
                 point2[axis] > (cellSize[axis] * numCells[axis]) - cellSize[axis]) ||
                (point2[axis] < 0 + cellSize[axis] &&
                 point1[axis] > (cellSize[axis] * numCells[axis]) - cellSize[axis])) {
                // for every axis where point1 and point2 are on opposite ends of the boundary, invert the distance vector
                // and subtract/add the distance between the boundaries to it
                v[axis] = v[axis] < 0 ?
                          v[axis] + (cellSize[axis] * numCells[axis])
                                      : v[axis] > 0 ?
                                        v[axis] - (cellSize[axis] * numCells[axis])
                                                    :
                                        v[axis];
            }
        }
    }
}

void ParticleContainerLinkedCell::computeNeighborCellsMatrix() {
    for (int z = 0; z < numCells[2]; z++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int x = 0; x < numCells[0]; x++) {
                std::array<int, 3> currCoords = {x, y, z};
                size_t currIdx = x + (y * numCells[0]) + (z * numCells[0] * numCells[1]);
                neighborCellsMatrix.push_back({});

                for (int offsetZ = 0; offsetZ < 2; offsetZ++) {
                    for (int offsetY = -1; offsetY < 2; offsetY++) {
                        for (int offsetX = -1; offsetX < 2; offsetX++) {
                            if (offsetY == -1 && offsetZ == 0) continue;
                            if (offsetX == -1 && offsetY == 0 && offsetZ == 0) continue;
                            std::array<int, 3> neighborCoords = ArrayUtils::elementWisePairOp(
                                    currCoords, {offsetX, offsetY, offsetZ}, std::plus<>());

                            if ((neighborCoords[0] < 0 && boundaryConfig.x[0] != periodic) ||
                                (neighborCoords[1] < 0 && boundaryConfig.y[0] != periodic) ||
                                (neighborCoords[2] < 0 && boundaryConfig.z[0] != periodic))
                                continue;

                            if ((neighborCoords[0] >= numCells[0] && boundaryConfig.x[1] != periodic) ||
                                (neighborCoords[1] >= numCells[1] && boundaryConfig.y[1] != periodic) ||
                                (neighborCoords[2] >= numCells[2] && boundaryConfig.z[1] != periodic))
                                continue;

                            // if coord is not out of bounds, use periodic behavior
                            int newX = (neighborCoords[0] + numCells[0]) % numCells[0];
                            int newY = (neighborCoords[1] + numCells[1]) % numCells[1];
                            int newZ = (neighborCoords[2] + numCells[2]) % numCells[2];

                            size_t neighborIdx = newX + (newY * numCells[0]) + (newZ * numCells[0] * numCells[1]);
                            neighborCellsMatrix[currIdx].push_back(neighborIdx);
                        }
                    }
                }
            }
        }
    }
}

std::vector<Cell> &ParticleContainerLinkedCell::getMesh() {
    return mesh;
}

std::vector<Particle> &ParticleContainerLinkedCell::getParticles() {
    return particles;
}

Cell &ParticleContainerLinkedCell::getCell(int idx) {
    return mesh[idx];
}

ParticleIterator ParticleContainerLinkedCell::begin() {
    return ParticleIterator(particles.begin(), particles.end());
}

ParticleIterator ParticleContainerLinkedCell::end() {
    return ParticleIterator(particles.end(), particles.end());
}

std::unique_ptr<PairParticleIterator> ParticleContainerLinkedCell::beginPairParticle() {
    return std::make_unique<PairParticleIteratorLinkedCell>(
            PairParticleIteratorLinkedCell(mesh.begin(), mesh.end(), mesh, particles, neighborCellsMatrix));
}

std::unique_ptr<PairParticleIterator> ParticleContainerLinkedCell::endPairParticle() {
    return std::make_unique<PairParticleIteratorLinkedCell>(
            PairParticleIteratorLinkedCell(mesh.end(), mesh.end(), mesh, particles, neighborCellsMatrix));
}

ParticleIteratorNonFixedParticles ParticleContainerLinkedCell::beginNonFixedParticles() {
    return ParticleIteratorNonFixedParticles(particles.begin(), particles.end());
}

ParticleIteratorNonFixedParticles ParticleContainerLinkedCell::endNonFixedParticles() {
    return ParticleIteratorNonFixedParticles(particles.end(), particles.end());
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::beginPairGhost() {
    return {mesh.begin(), mesh.end(), mesh, numCells, cellSize, boundaryConfig, particles};
}

PairParticleIteratorBoundaryNHalo ParticleContainerLinkedCell::endPairGhost() {
    return {mesh.end(), mesh.end(), mesh, numCells, cellSize, boundaryConfig, particles};
}

PairParticleIteratorMembraneDirectNeighbor ParticleContainerLinkedCell::beginMembraneDirectNeighbor() {

    return {particles.begin(), particles.end(), particles};
}

PairParticleIteratorMembraneDirectNeighbor ParticleContainerLinkedCell::endMembraneDirectNeighbor() {
    auto test = particles.end();
    SPDLOG_DEBUG("first end created");

    test = particles.end();
    SPDLOG_DEBUG("second iterator created");

    return {particles.end(), particles.end(), particles};
}

PairParticleIteratorMembraneDiagonalNeighbor ParticleContainerLinkedCell::beginMembraneDiagonalNeighbor() {
    return {particles.begin(), particles.end(), particles};
}

PairParticleIteratorMembraneDiagonalNeighbor ParticleContainerLinkedCell::endMembraneDiagonalNeighbor() {
    return {particles.end(), particles.end(), particles};
}

int ParticleContainerLinkedCell::size() {
    int size = 0;
    for (auto &i: mesh) {
        size += i.getParticlesIndices().size();
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

Particle &ParticleContainerLinkedCell::getParticleAt(size_t particleIndex) {
    return particles.at(particleIndex);
}

std::array<double, 3> ParticleContainerLinkedCell::getDomainSize() {
    return domainSize;
}

struct boundaryConfig ParticleContainerLinkedCell::getBoundaryConfig() {
    return boundaryConfig;
}
