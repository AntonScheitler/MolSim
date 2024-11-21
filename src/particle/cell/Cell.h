#include "vector"
#include <particle/Particle.h>

class Cell {

public:
    Cell();
    Cell(bool isBoundary, bool isHalo);
    void addParticle(const Particle& particle);
    bool isBoundary();
    bool isHalo();
    void setBoundary(bool boundary);
    void setHalo(bool halo);
    int size();
    std::vector<Particle> getParticles();
    void removeParticle(const Particle& particle);

private:
    std::vector<Particle> particles;

    bool boundaryCell;
    bool haloCell;
};
