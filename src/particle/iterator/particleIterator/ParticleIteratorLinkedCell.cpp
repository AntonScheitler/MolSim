#include "ParticleIteratorLinkedCell.h"

ParticleIteratorLinkedCell::ParticleIteratorLinkedCell(std::vector<Cell>::iterator it, std::vector<Cell>::iterator endArg) {
    currentCell = it;
    end = endArg;
    currentParticle = currentCell->getParticles().begin();
}

ParticleIteratorLinkedCell::reference ParticleIteratorLinkedCell::operator*() {
    return *currentParticle;
}

ParticleIteratorLinkedCell &ParticleIteratorLinkedCell::operator++() {
    ++currentParticle;
    if (currentParticle == currentCell->getParticles().end()) {
        ++currentCell;
        if (currentCell == end) {
            return *this;
        }
        currentParticle = currentCell->getParticles().begin();
    }
    return *this;
}

bool ParticleIteratorLinkedCell::operator!=(const ParticleIteratorLinkedCell &other) {
    return currentCell != other.currentCell;
}