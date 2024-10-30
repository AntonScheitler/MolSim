#include "particle/ParticleContainer.h"

/**
 * namespace that wraps different writers which write simulation data into file. All of those writers inherit from the abstract FileWriter
 */
namespace outputWriter {
    /**
     * @brief a superclass for all filewriters, which are each specialized in different output formats
     */
    class FileWriter {
        public:
            FileWriter() = default;
            virtual ~FileWriter() = default;
            /**
             * @brief plot particles and write them to a file 
             * @param particles the particles to be plotted and written
             * @param iteration the iteration the simulation is currently in
             */
            virtual void plotParticles(ParticleContainer &particles, int iteration) = 0;
    };
} // namespace outputWriter
