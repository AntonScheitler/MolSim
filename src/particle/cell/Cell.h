#include "vector"
#include <particle/Particle.h>

class Cell {

public:

    Cell();
    //Cell(std::array<double, 3> posBottomLeft, std::array<double, 3> cellDims);
    void addParticle(const Particle& particle);
    bool isBoundary();
    bool isHalo();
    void setBoundary(bool boundary);
    // TODO maybe better to have a separate Halo cell class?
    void setHalo(bool halo);

private:
    std::vector<Particle> particles;
    // TODO does this need to exist?
    //std::array<double, 3> posBottomLeft;
    //std::array<double, 3> cellDims;
    bool boundaryCell;
    bool haloCell;
};
