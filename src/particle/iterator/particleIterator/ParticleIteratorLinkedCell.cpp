#include "ParticleIteratorLinkedCell.h"

ParticleIteratorLinkedCell::ParticleIteratorLinkedCell(std::vector<Cell>::iterator it) {
    currentCell = it;
    currentParticle = currentCell->getParticles().begin();
}

ParticleIteratorLinkedCell::reference ParticleIteratorLinkedCell::operator*() {
    return *currentParticle;
}

ParticleIteratorLinkedCell &ParticleIteratorLinkedCell::operator++() {
    ++currentParticle;
    if (currentParticle == currentCell->getParticles().end()) {
        ++currentCell;
        // TODO this maybe causes an error??, since currentCell will not have particles, if it is at the end??
        currentParticle = currentCell->getParticles().begin();
    }
    return *this;
}

bool ParticleIteratorLinkedCell::operator!=(const ParticleIteratorLinkedCell &other) {
    return (currentCell != other.currentCell) || (currentParticle != other.currentParticle);
}
