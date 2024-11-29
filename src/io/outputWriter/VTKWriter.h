/*
 * VTKWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "XYZWriter.h"
#include "vtk-unstructured.h"
#include "particle/ParticleContainer.h"

namespace outputWriter {
    /**
     * @brief This class implements the functionality to generate vtk output from
     * particles.
     */
    class VTKWriter : public FileWriter {
        public:

            VTKWriter(std::string baseName);
            /**
             * @brief plots particles and writes them to a vtk file 
             */
            void plotParticles(ParticleContainer &particles, int iteration);

        private:

            std::string baseName = "MD_vtk";
            std::shared_ptr<spdlog::logger> logger;

            /**
             * @brief set up internal data structures and prepare to plot a particle.
             */
            void initializeOutput(int numParticles);

            /**
             * @brief plot type, mass, position, velocity and force of a particle.
             *
             * @note: initializeOutput() must have been called before.
             */
            void plotParticle(Particle &p);

            /**
             * @brief writes the final output file.
             *
             * @param filename the base name of the file to be written.
             * @param iteration the number of the current iteration,
             *        which is used to generate an unique filename
             */
            void writeFile(const std::string &filename, int iteration);

            VTKFile_t *vtkFile{};
    };

} // namespace outputWriter
