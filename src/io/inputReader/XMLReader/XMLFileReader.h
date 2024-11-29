#include "simulation/SimulationData.h"
#include <iostream>
#include <fstream>
#include "spdlogConfig.h"
#include "io/inputReader/xml/simulation.h"
#include <array>
#include "io/inputReader/ParticleGenerator.h"


namespace inputReader {
    /**
     * @brief a file reader specifically for json files following a certain format
     */
    class XMLFileReader {
    public:
        /**
         * @brief creates an instance of the JsonFileReader
         */
        XMLFileReader(SimulationData &simDataArg);

        /**
         * @brief destructs an instance of the JsonFileReader
         */
        ~XMLFileReader();

        /**
         * @brief xml file reader specifically for the Collision simulation
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        void readCollisionFile(SimulationData& simData, char *filename);

        /**
         * @brief xml file reader specifically for the Comet simulation
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        void readCometFile(SimulationData& simData, char *filename);

        void readDropFile(SimulationData& simData, char *filename);


    private:

        /**
         * @brief a specific logger for the XMLFileReader
         */
        std::shared_ptr<spdlog::logger> logger;

        SimulationData &simData;

        void readParams();
    };
} // namespace inputReader




