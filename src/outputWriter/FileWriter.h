#include "particle/Particle.h"
#include <vector>

namespace outputWriter {
    class FileWriter {
        public:
            FileWriter() = default;
            virtual ~FileWriter() = default;
            virtual void plotParticles(std::vector<Particle> &particles, int iteration) = 0;
    };
} // namespace outputWriter
