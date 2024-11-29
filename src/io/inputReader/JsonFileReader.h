#include "particle/container/ParticleContainer.h"
#include "spdlogConfig.h"
#include <simulation/SimulationData.h>


namespace inputReader {
    /**
     * @brief a file reader specifically for json files following a certain format
     */
    class JsonFileReader {
    public:
        /**
         * @brief creates an instance of the JsonFileReader
         */
        JsonFileReader(SimulationData& simDataArg);
        /**
         * @brief destructs an instance of the JsonFileReader
         */
        ~JsonFileReader();
        /**
         * @brief json file reader specifically for the Collision simulation
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        void readFile(SimulationData& simData, char *filename);
        /**
         * @brief json file reader specifically for the Comet simulation
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */


    private:

        /**
         * @brief a specific logger for the JsonFileReader
         */
        std::shared_ptr<spdlog::logger> logger;
        SimulationData simData;
    };
} // namespace inputReader
