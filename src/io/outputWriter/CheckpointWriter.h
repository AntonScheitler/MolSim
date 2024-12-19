#pragma once

#include <particle/container/ParticleContainer.h>
#include "spdlogConfig.h"
#include "FileWriter.h"


namespace outputWriter {

    class CheckpointWriter : public FileWriter {
    public:

        CheckpointWriter(std::string baseName);
        /**
         * @brief plots particles and writes them to a vtk file
         */
        void plotParticles(ParticleContainer &particles, int iteration);

    private:

        std::string baseName = "Checkpoint";
        std::shared_ptr<spdlog::logger> logger;


        /**
         * @brief plot type, mass, position, velocity and force of a particle.
         *
         * @note: initializeOutput() must have been called before.
         */
        std::string plotParticle(Particle &p);



    };

} // namespace outputWriter
