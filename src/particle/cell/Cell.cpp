#include "Cell.h"
#include <bits/stdc++.h>
#include "spdlogConfig.h"


Cell::Cell(bool isBoundaryArg, size_t cellIdArg) {
    isBoundary = isBoundaryArg;
    cellId = cellIdArg;
}

void Cell::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

std::vector<Particle> &Cell::getParticles() {
    return particles;
}

void Cell::removeParticle(Particle &particle) {
    auto it = std::find(particles.begin(), particles.end(), particle);
    particles.erase(it);
}

int Cell::size() {
    return particles.size();
}

bool Cell::operator==(const Cell &other) {
    return cellId == other.cellId;
}
