/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/ParticleContainer.h"

#include <vector>


namespace inputReader {

/**
 * @brief superclass for all file readers
 */
    class FileReader {

    public:
        FileReader() = default;

        virtual ~FileReader() = default;
        virtual void readFile(ParticleContainer &particles, char *filename) = 0;
    };
}
