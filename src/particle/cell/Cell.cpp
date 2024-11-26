#include "Cell.h"
#include <bits/stdc++.h>

Cell::Cell() {
    this->boundaryCell = false;
    this->haloCell = false;
}

Cell::Cell(bool isBoundary, bool isHalo) {
    this->boundaryCell = isBoundary;
    this->haloCell = isHalo;
}

bool Cell::isHalo() {
    return haloCell;
}
void Cell::setHalo(bool halo) {
    this->haloCell = halo;
}
bool Cell::isBoundary() {
    return boundaryCell;
}
void Cell::setBoundary(bool boundary) {
    this->boundaryCell = boundary;
}

void Cell::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

int Cell::size() {
    return particles.size();
}

std::vector<Particle> Cell::getParticles() {
    return particles;
}

void Cell::removeParticle(Particle &particle) {
    // TODO: fix
//    particles.erase(std::find(particles.begin(), particles.end(), particle));
}
