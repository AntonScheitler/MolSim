#include "ParticleContainerLinkedCell.h"
#include "../utils/ArrayUtils.h"


ParticleContainerLinkedCell::ParticleContainerLinkedCell(const std::vector<Particle> &particlesArg,
                                                         BoundaryCondition boundaryCondition,
                                                         std::vector<double> domainSize, double cutoffRadius) {


}

void ParticleContainerLinkedCell::initMesh(std::array<double, 3> domainSize, double cutoffRadius) {
    std::array<double, 3> cellSize{cutoffRadius, cutoffRadius, 1};

    std::array<double, 3> cellNumberNotRounded = ArrayUtils::elementWisePairOp(domainSize, cellSize, std::divides<>());
    for (int i = 0; i < 3; i++) {
        cellDimsNumber[i] = ceil(cellDimsNumber[i]);
        cellSize[i] = domainSize[i] / cellDimsNumber[i];
    }

    this->cellDims = cellSize;

    for (int i = 0; i < cellDimsNumber[0]; i++) {
        for (int j = 0; j < cellDimsNumber[1]; j++) {
            for (int k = 0; k < cellDimsNumber[2]; k++) {
                bool isBoundary = i == 0 || j == 0 || k == 0
                        || i == cellDimsNumber[0] - 1 || j == cellDimsNumber[1] - 1 || k == cellDimsNumber[2] - 1;
                Cell cell{std::array<double, 3>{i * cellSize[i], j * cellSize[j], k * cellSize[k]}, cellSize};
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
    return coord[0] * cellDimsNumber[1] * cellDimsNumber[2] + coord[1] * cellDimsNumber[2] + coord[2];
}


int ParticleContainerLinkedCell::continuousCoordsToIndex(std::array<double, 3> coord) {
    std::array<double, 3> discreteCoords = ArrayUtils::elementWisePairOp(coord, cellDims, std::divides<>());
    return discreteCoordsToIndex(std::array<int, 3>{(int) floor(discreteCoords[0]), (int) floor(discreteCoords[1]),
                                                    (int) floor(discreteCoords[2])});
}

//std::array<double, 3> indexToContinuousCoords(int index) {
//    return std::array<int, 3>
//}


