#pragma once
#include "vector"
#include <particle/Particle.h>

class Cell {

public:
    Cell(bool isBoundaryCell, size_t cellIdArg);
    void addParticle(const Particle& particle);
    void removeParticle(Particle &particle);
    std::vector<Particle>& getParticles();
    bool isBoundary;
    size_t cellId;
    int size();
    bool operator==(const Cell &other);
private:
    std::vector<Particle> particles;
};
