/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/ParticleContainer.h"
#include "spdlogConfig.h"
#include <memory>

/**
 * @brief namespace that wraps different readers which parse input from a file. All of those readers inherit from the abstract FileReader
 */
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
        private:
            std::shared_ptr<spdlog::logger> logger;

    };
} // namespace inputReader