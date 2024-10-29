#include "particle/Particle.h"
#include <vector>

namespace outputWriter {
    class FileWriter {
        public:
            FileWriter() = default;
            virtual ~FileWriter() = default;
            virtual void plotParticles(std::vector<Particle> &particles,
                    const std::string &filename, int iteration) = 0;
    };
} // namespace outputWriter
