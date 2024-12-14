/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/container/ParticleContainer.h"
#include "./FileWriter.h"

namespace outputWriter {
    /**
     * @brief file writer, which specializes in xyz outputs
     */
    class XYZWriter : public FileWriter {

        public:
            /** 
             * @brief plots the particles and writes them to an xyz file
             */
            void plotParticles(ParticleContainer &particles, int iteration);
    };

} // namespace outputWriter
