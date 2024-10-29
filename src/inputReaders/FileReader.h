/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "particle/Particle.h"

#include <list>


namespace inputReader {

/**
 * @brief superclass for all file readers
 */
class FileReader {

public:
  FileReader() = default;
  virtual ~FileReader() = default;
  // todo rebuild to use particlecontainer
  virtual void readFile(std::list<Particle> &particles, char *filename) = 0;
};
}
