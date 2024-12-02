#include "simulation/SimulationData.h"
#include "spdlogConfig.h"


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
        void readCollisionFile(char *filename);
        /**
         * @brief xml file reader specifically for the Comet simulation
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        void readCometFile(char *filename);
        void readDropFile(char *filename);


    private:

        /**
         * @brief a specific logger for the XMLFileReader
         */
        std::shared_ptr<spdlog::logger> logger;

        SimulationData &simData;

        void readParams();
    };
} // namespace inputReader
