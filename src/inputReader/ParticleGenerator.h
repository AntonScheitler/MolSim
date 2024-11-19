#include "../particle/ParticleContainerDirectSum.h"


namespace inputReader {
    class ParticleGenerator {
    public:
        static void generateParticles(ParticleContainer &particles, std::array<double, 3> x, std::array<double, 3> v,
                                      std::array<int, 3> d, double m, double h, int type);
    };

}