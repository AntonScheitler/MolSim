#include "../../particle/container/ParticleContainer.h"
#include "ParticleGenerator.h"

namespace inputReader {


    void
    ParticleGenerator::generateCuboid(ParticleContainer &particles, std::array<double, 3> x, std::array<double, 3> v,
                                      std::array<int, 3> d, double m, double h, int type) {
        std::array<double, 3> tempx{};
        for (int j = 0; j < d[0]; ++j) {
            tempx[0] = j * h + x[0];
            for (int k = 0; k < d[1]; ++k) {
                tempx[1] = k * h + x[1];
                for (int l = 0; l < d[2]; ++l) {
                    tempx[2] = l * h + x[2];

                    particles.addParticle(Particle(tempx, v, m, type));
                }
            }
        }
    }

    void ParticleGenerator::generateDisc(ParticleContainer &particles, std::array<double, 3> center,
                                         std::array<double, 3> v, double r, double m, double h, int type) {
        //todo
        std::array<double, 3> tempx{};
        for (double x = -r; x <= r; x += h) {
            for (double y = -r; y <= r; y += h) {
                if (x * x + y * y <= r * r) {  // Check if the point is within the disc
                    tempx[0] = center[0] + x;
                    tempx[1] = center[1] + y;
                    tempx[2] = center[2];  // Z-coordinate remains fixed
                    particles.addParticle(Particle(tempx, v, m, type));
                }
            }
        }


    }
}