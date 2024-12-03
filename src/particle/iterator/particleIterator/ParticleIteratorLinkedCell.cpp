#include "ParticleIteratorLinkedCell.h"
#include "particle/iterator/particleIterator/ParticleIterator.h"


ParticleIteratorLinkedCell::ParticleIteratorLinkedCell(std::vector<Cell>::iterator it,
                                                       std::vector<Cell>::iterator endArg) {
    currentCell = it;
    end = endArg;
    stepToNonEmptyCell();
    if (it == endArg) {
        return;
    }
    currentParticle = currentCell->getParticles().begin();
}

ParticleIteratorLinkedCell::reference ParticleIteratorLinkedCell::operator*() {
    return *currentParticle;
}

ParticleIteratorLinkedCell &ParticleIteratorLinkedCell::operator++() {
    ++currentParticle;
    if (currentParticle == currentCell->getParticles().end()) {
        ++currentCell;
        stepToNonEmptyCell();
        if (currentCell == end) {
            return *this;
        }
    }
    return *this;
}

bool ParticleIteratorLinkedCell::operator!=(const ParticleIterator &other) {
    auto casted = dynamic_cast<const ParticleIteratorLinkedCell *>(&other);
    if (casted) {
        return currentCell != casted->currentCell;
    }
    return true;
}

void ParticleIteratorLinkedCell::stepToNonEmptyCell() {
    while (currentCell != end && currentCell->getParticles().size() == 0) {
        ++currentCell;
    }
    if (currentCell != end) {
        currentParticle = currentCell->getParticles().begin();
    }
}
