#pragma once

#include <particle/container/ParticleContainer.h>
#include "spdlogConfig.h"
#include "FileWriter.h"


namespace outputWriter {
    /**
  * @brief This class implements the functionality to create a checkpoint file
  */
    class CheckpointWriter : public FileWriter {
    public:

        CheckpointWriter(std::string baseName);

        /**
         * @brief plots particles and writes them to a vtk file
         */
        void plotParticles(ParticleContainer &particles, int iteration);

    private:

        std::string baseName = "checkpoint_File";
        std::shared_ptr<spdlog::logger> logger;


        /**
         * @brief plot type, mass, position, velocity and force of a particle.
         */
        std::string plotParticle(Particle &p);


    };

} // namespace outputWriter
