#include "Cell.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <cstdlib>
#include "spdlog/spdlog.h"
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

void Cell::removeParticle(size_t particleId) {
    auto it = std::find_if(particles.begin(), particles.end(),
                   [particleId](const Particle& particle) { return particle.getId() == particleId; });
    if (it == particles.end()) {
        SPDLOG_ERROR("SOMETHING VERY TERRIBLE HAS HAPPENED :(");
        exit(EXIT_FAILURE);
    }
    particles.erase(it);
}

int Cell::size() {
    return particles.size();
}

bool Cell::operator==(const Cell &other) {
    return cellId == other.cellId;
}
