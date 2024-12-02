#pragma once
#include "vector"
#include <particle/Particle.h>

class Cell {

public:
    Cell(bool isBoundaryCell);
    void addParticle(const Particle& particle);
    void removeParticle(Particle &particle);
    std::vector<Particle>& getParticles();
    bool isBoundary;
    int size();
private:
    std::vector<Particle> particles;
};
