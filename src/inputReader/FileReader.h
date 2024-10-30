/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/ParticleContainer.h"

namespace inputReader {

    /**
     * @brief superclass for all file readers
     */
    class FileReader {

        public:
            FileReader() = default;

            virtual ~FileReader() = default;
            /**
             * @brief reads a file and fills a ParticleContainer accordingly
             * @param particles the particles to be filled up
             * @param filename the file to read the particle information from
             */
            virtual void readFile(ParticleContainer &particles, char *filename) = 0;
    };
} // namespace inputReader
