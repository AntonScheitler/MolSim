/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/ParticleContainer.h"
#include "./FileWriter.h"

#include <fstream>
#include <list>
#include <vector>

namespace outputWriter {
    class XYZWriter : public FileWriter {

    public:
        void plotParticles(ParticleContainer &particles, int iteration);
    };

} // namespace outputWriter
