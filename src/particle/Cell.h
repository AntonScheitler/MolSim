#include "vector"
#include "Particle.h"

class Cell {

public:

    Cell(std::array<double, 3> posBottomLeft, std::array<double, 3> cellDims);
    void addParticle(const Particle& particle);
    bool isBoundary();
    bool isHalo();
    void setBoundary(bool boundary);
    void setHalo(bool halo);

private:
    std::vector<Particle> particles;
    std::array<double, 3> posBottomLeft;
    std::array<double, 3> cellDims;
    bool boundaryCell;
    bool haloCell;
};