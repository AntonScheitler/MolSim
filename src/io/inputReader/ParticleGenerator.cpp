#include "../../particle/container/ParticleContainer.h"
#include "ParticleGenerator.h"
#include <cmath>
#include "spdlogConfig.h"


namespace inputReader {
    void
    ParticleGenerator::generateCuboid(ParticleContainer &particles, std::array<double, 3> x, std::array<double, 3> v,
                                      std::array<int, 3> d, double m, double h, int type, double e, double s) {
        int currIndex = 0;
        std::array<double, 3> tempx{};
        for (int j = 0; j < d[0]; ++j) {
            tempx[0] = j * h + x[0];
            for (int k = 0; k < d[1]; ++k) {
                tempx[1] = k * h + x[1];
                for (int l = 0; l < d[2]; ++l) {
                    tempx[2] = l * h + x[2];

                    Particle temp = Particle(tempx, v, m, type);
                    temp.setSigma(s);
                    temp.setEpsilon(e);

                    particles.addParticle(temp);
                }
            }
        }
    }

    void ParticleGenerator::generateDisc(ParticleContainer &particles, std::array<double, 3> center,
                                         std::array<double, 3> v, double r, double m, double h, int type, double e, double s) {

        SPDLOG_INFO("starting disc generator");
        for (int i = 0; i <= r; ++i) {
            double currentRadius = i * h;

            if (i == 0) {
                particles.addParticle(Particle(center, v, m, type));
                continue;
            }
            SPDLOG_INFO("adding particle in disc generator");

            int particlesPerRing = static_cast<int>(2 * M_PI * currentRadius / h);

            for (int j = 0; j < particlesPerRing; ++j) {
                double angle = (2 * M_PI / particlesPerRing) * j;

                double x = currentRadius * cos(angle);
                double y = currentRadius * sin(angle);

                std::array<double, 3> tempx = {center[0] + x, center[1] + y, center[2]};
                Particle temp = Particle(tempx, v, m, type);
                temp.setSigma(s);
                temp.setEpsilon(e);
                particles.addParticle(temp);
            }
        }
        SPDLOG_INFO("ending disc generator");
    }

    void ParticleGenerator::addNeighbors(Particle &particle, int j, int k, int l, std::array<int, 3> d) {
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                for (int z = -1; z <= 1; ++z) {
                    if (x == 0 && y == 0 && z == 0) {
                        continue;
                    }

                    int neighborX = j + x;
                    int neighborY = k + y;
                    int neighborZ = l + z;


                    if (neighborX >= 0 && neighborX < d[0] &&
                        neighborY >= 0 && neighborY < d[1] &&
                        neighborZ >= 0 && neighborZ < d[2]) {

                        int neighborIndex = neighborX * d[1] * d[2] +
                                            neighborY * d[2] +
                                            neighborZ;


                        int diffCount = abs(x) + abs(y) + abs(z);
                        if (diffCount == 1) {
                            temp.addDirectNeighbor(neighborIndex);
                        } else {
                            temp.addDiagonalNeighbor(neighborIndex);
                        }
                    }
                }
            }
        }

    }

}
