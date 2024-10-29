/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/Particle.h"
#include "./FileWriter.h"

#include <fstream>
#include <list>
#include <vector>

namespace outputWriter {
    class XYZWriter : public FileWriter {

        public:
            void plotParticles(std::vector<Particle> &particles, const std::string &filename,
                    int iteration);
    };

} // namespace outputWriter
