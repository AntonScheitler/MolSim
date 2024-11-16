
#include "ParticleGenerator.h"




namespace inputReader {


     void ParticleGenerator::generateParticles(ParticleContainer &particles, std::array<double, 3> x, std::array<double, 3> v,
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


}