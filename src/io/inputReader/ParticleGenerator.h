#pragma once

namespace inputReader {
    class ParticleGenerator {
    public:
        static void generateCuboid(ParticleContainer &particles, std::array<double, 3> x, std::array<double, 3> v,
                                      std::array<int, 3> d, double m, double h, int type);

        static void generateDisc(ParticleContainer &particle, std::array<double, 3> center,
                                            std::array<double, 3> v, double r, double m, double h, int type);

        };

}