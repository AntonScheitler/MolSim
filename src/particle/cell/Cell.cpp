#include "Cell.h"

Cell::Cell() {
    this->boundaryCell = false;
    this->haloCell = false;
}

Cell::Cell(bool isBoundary, bool isHalo) {
    this->boundaryCell = isBoundary;
    this->haloCell = isHalo;
}
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

int Cell::size() {
    return particles.size();
}




