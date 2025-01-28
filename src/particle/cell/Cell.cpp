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

void Cell::addParticleIdx(const size_t idx) {
    //auto it = std::find_if(particleIndices.begin(), particleIndices.end(),
    //                       [idx](const size_t currIdx) { return currIdx == idx; });
    //if (it != particleIndices.end()) {
    //    SPDLOG_ERROR("SOMETHING TERRIBLE HAS HAPPENED :(");
    //    exit(EXIT_FAILURE);
    //}
    particleIndices.push_back(idx);
}

std::vector<size_t> &Cell::getParticlesIndices() {
    return particleIndices;
}

void Cell::removeParticle(size_t deleteIdx) {
    auto it = std::find_if(particleIndices.begin(), particleIndices.end(),
                           [deleteIdx](const size_t currIdx) { return currIdx == deleteIdx; });
    if (it == particleIndices.end()) {
        SPDLOG_ERROR("SOMETHING VERY TERRIBLE HAS HAPPENED :(");
        exit(EXIT_FAILURE);
    }
    particleIndices.erase(it);
}

int Cell::size() {
    return particleIndices.size();
}

bool Cell::operator==(const Cell &other) {
    return cellId == other.cellId;
}
