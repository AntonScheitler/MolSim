#include "ParticleContainerLinkedCell.h"
#include <utils/ArrayUtils.h>

ParticleContainerLinkedCell::ParticleContainerLinkedCell(const std::vector<Particle> &particlesArg,
        std::vector<double> domainSize, double cutoffRadius) {
    // TODO
}

// TODO move to constructor?
void ParticleContainerLinkedCell::initMesh(std::array<double, 3> domainSize, double cutoffRadius) {
    // initialize size for cells
    cellSize = {cutoffRadius, cutoffRadius, 1};

    // adjust cell size depending on the domain size
    for (int i = 0; i < 3; i++) {
        numCells[i] = ceil(domainSize[i] / cellSize[i]);
        // TODO is 1.0 *... necessary??
        cellSize[i] = domainSize[i] / (1.0 * numCells[i]);
    }

    for (int x = 0; x < numCells[0]; x++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int z = 0; z < numCells[2]; z++) {
                bool isBoundary = x == 0 || y == 0 || z == 0
                    || x == numCells[0] - 1 || y == numCells[1] - 1 || z == numCells[2] - 1;
                // TODO redo cells
                Cell cell = Cell();
                cell.setBoundary(isBoundary);
                mesh.push_back(cell);
            }
        }
    }
}

void ParticleContainerLinkedCell::addParticle(const Particle &particle) {
    mesh[continuousCoordsToIndex(particle.getX())].addParticle(particle);
}

int ParticleContainerLinkedCell::discreteCoordsToIndex(std::array<int, 3> coord) {
    return (coord[0] * numCells[1] * numCells[2]) + (coord[1] * numCells[2]) + coord[2];
}

int ParticleContainerLinkedCell::continuousCoordsToIndex(std::array<double, 3> coord) {
    std::array<double, 3> approxDiscreteCoords = ArrayUtils::elementWisePairOp(coord, cellSize, std::divides<>());
    return discreteCoordsToIndex(std::array<int, 3>{(int) floor(approxDiscreteCoords[0]), (int) floor(approxDiscreteCoords[1]), (int) floor(approxDiscreteCoords[2])});
}
