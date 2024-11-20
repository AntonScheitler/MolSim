#include "Cell.h"
void Cell::setHalo(bool halo) {
    this->haloCell = halo;
}

bool Cell::isHalo() {
    return this->haloCell;
}

void Cell::setBoundary(bool boundary) {
    this->boundaryCell = boundary;
}

bool Cell::isBoundary() {
    return this->boundaryCell;
}

void Cell::addParticle(const Particle &particle) {
    particles.push_back(particle);
}


