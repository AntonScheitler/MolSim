#include "particle/ParticleContainer.h"
#include <vector>

namespace outputWriter {
    class FileWriter {
        public:
            FileWriter() = default;
            virtual ~FileWriter() = default;
            virtual void plotParticles(ParticleContainer &particles, int iteration) = 0;
    };
} // namespace outputWriter
