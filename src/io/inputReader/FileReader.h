/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "spdlogConfig.h"
#include "simulation/SimulationData.h"
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
        /**
         * @brief creates an instance of a FileReader
         * @param simDataArg the simulationData to write particle information into
         */
        FileReader(SimulationData &simDataArg);

        /**
         * @brief destructor for a FileReader instance
         */
        ~FileReader();

        /**
         * @brief reads a file and fills a ParticleContainer accordingly
         * @param particles the particles to be filled up
         * @param filename the file to read the particle information from
         */
        void readFile(char *filename);

    private:
        /**
         * @brief the simulation data to write the information into
         */
        SimulationData &simData;
        /**
         * @brief a fileReader-specific logger
         */
        std::shared_ptr<spdlog::logger> logger;

        /**
         * @brief reads a json file and writes the particle information into the particleContainer
         * @param particles the particleContainer to write into
         * @param filename the file to read from
         */
        void readJson(SimulationData &simData, char *filename);

        /**
         * @brief reads a txt file and  writes the simulation information into the given simData
         * @param simData the SimulationData to write into
         * @param filename the name of the file to read from
         */
        void readTxt(SimulationData &simData, char *filename);

        /**
         * @brief reads a xml file and writes the simulation information into the given simData
         * @param simData the SimulationData to write into
         * @param filename the name of the file to read from
         */
        void readXML(SimulationData &simData, char *filename);
    };
} // namespace inputReader
