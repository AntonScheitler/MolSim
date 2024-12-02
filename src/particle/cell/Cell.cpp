#include "Cell.h"
#include <bits/stdc++.h>
#include "spdlogConfig.h"

Cell::Cell(bool isBoundaryArg) {
    isBoundary = isBoundaryArg;
}

void Cell::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

std::vector<Particle> &Cell::getParticles() {
    return particles;
}

void Cell::removeParticle(Particle &particle) {
    auto it = std::find(
            particles.begin(), particles.end(), particle);
    if (it != particles.end()) {
        particles.erase(it);
    } else
        SPDLOG_WARN("cannot remove particle from cell because it is not contained");
}

int Cell::size() {
    return particles.size();
}
