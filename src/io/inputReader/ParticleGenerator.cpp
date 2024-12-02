#include "../../particle/container/ParticleContainer.h"
#include "ParticleGenerator.h"
#include <math.h>
#include "spdlogConfig.h"


namespace inputReader {
    void
    ParticleGenerator::generateCuboid(ParticleContainer& particles, std::array<double, 3> x, std::array<double, 3> v,
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

        SPDLOG_INFO("starting disc generator");
        for (int i = 0; i <= r; ++i) {
            double currentRadius = i * h;

            if (i == 0) {
                particles.addParticle(Particle(center, v, m, type));
                continue;
            }
            SPDLOG_INFO("adding particle in disc generator");

            int particlesPerRing =  static_cast<int>(2 * M_PI * currentRadius / h);

            for (int j = 0; j < particlesPerRing; ++j) {
                double angle = (2 * M_PI  / particlesPerRing) * j;

                double x = currentRadius * cos(angle);
                double y = currentRadius * sin(angle);

                std::array<double, 3> tempx = {center[0] + x, center[1] + y, center[2]};
                particles.addParticle(Particle(tempx, v, m, type));
            }
        }
        SPDLOG_INFO("ending disc generator");
    }
}
