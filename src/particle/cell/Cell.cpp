#include "Cell.h"

Cell::Cell(bool isBoundaryArg) {
    isBoundary = isBoundaryArg;
}

void Cell::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

std::vector<Particle>& Cell::getParticles() {
    return particles;
}
