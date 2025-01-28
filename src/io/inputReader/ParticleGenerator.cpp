#include "../../particle/container/ParticleContainer.h"
#include "ParticleGenerator.h"
#include <cmath>
#include "spdlogConfig.h"

namespace inputReader {
    void
    ParticleGenerator::generateCuboid(ParticleContainer &particles, std::array<double, 3> x, std::array<double, 3> v,
                                      std::array<int, 3> d, double m, double h, int type, double e, double s, bool fixed) {


        std::array<double, 3> tempx{};
        for (int dz = 0; dz < d[2]; ++dz) {
            tempx[2] = dz * h + x[2];
            for (int dy = 0; dy < d[1]; ++dy) {
                tempx[1] = dy * h + x[1];
                for (int dx = 0; dx < d[0]; ++dx) {
                    tempx[0] = dx * h + x[0];

                    Particle temp = Particle(tempx, v, m, type, e, s, fixed);
                    temp.setSigma(s);
                    temp.setEpsilon(e);
                    addNeighbors(temp, dx, dy, dz, d);

                    particles.addParticle(temp);
                    SPDLOG_DEBUG("adding particle at {0}, {1}, {2}", tempx[0], tempx[1], tempx[2]);
                }
            }
        }
    }



    void ParticleGenerator::addNeighbors(Particle &particle, int dx, int dy, int dz, std::array<int, 3> d) {
        for (int z = 0; z <= 0; ++z) {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    if (x == 0 && y == 0 && z == 0) {
                        continue;
                    }

                    int neighborZ = dz + z;
                    int neighborY = dy + y;
                    int neighborX = dx + x;


                    if (neighborX >= 0 && neighborX < d[0] &&
                        neighborY >= 0 && neighborY < d[1] &&
                        neighborZ >= 0 && neighborZ < d[2]) {

                        int neighborIndex = neighborX +
                                            neighborY * d[0] +
                                            neighborZ * d[1] * d[0];




                        int diffCount = (x != 0) + (y != 0) + (z != 0);
                        if (diffCount == 1) {
                            particle.addNeighborIdx(neighborIndex);
                        } else {
                            particle.addDiagNeighborIdx(neighborIndex);
                        }
                    } else {
                        int diffCount = (x != 0) + (y != 0) + (z != 0);
                        if (diffCount == 1) {
                            particle.addNeighborIdx(-1);
                        } else {
                            particle.addDiagNeighborIdx(-1);
                        }
                    }
                }
            }
        }
    }

    void ParticleGenerator::generateDisc(ParticleContainer &particles, std::array<double, 3> center,
                                         std::array<double, 3> v, double r, double m, double h, int type, double e, double s, bool fixed) {

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

                temp.setFixed(fixed);
                particles.addParticle(temp);
            }
        }
        SPDLOG_INFO("ending disc generator");
    }
}
