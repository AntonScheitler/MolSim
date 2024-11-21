#include "vector"
#include <particle/Particle.h>

class Cell {

public:
    Cell(bool isBoundaryCell);
    void addParticle(const Particle& particle);
    std::vector<Particle>& getParticles();
    bool isBoundary;
private:
    std::vector<Particle> particles;
};
