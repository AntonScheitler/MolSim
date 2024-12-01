#include "spdlogConfig.h"
#include <simulation/SimulationData.h>

namespace inputReader {
    /**
     * @brief a file reader specifically for txt files following a certain format
     */
    class TxtFileReader{
        public:
            /**
             * @brief creates an instance of the TxtFileReader
             */
            TxtFileReader(SimulationData& simulationDataArg);
            /**
             * @brief destructs an instance of the TxtFileReader
             */
            ~TxtFileReader();
            /**
             * @brief txt file reader specifically for the Comet Simulation
             * @param particles the particles to store the information in
             * @param filename the name of the file to read from
             */
            void readCometFile(SimulationData& simData, char *filename) ;
            /**
             * @brief txt file reader specifically for the Collision Simulation
             * @param particles the particles to store the information in
             * @param filename the name of the file to read from
             */
            void readCollisionFile(SimulationData& simData, char *filename) ;
        private:

            /**
             * @brief a specific logger for the TxtFileReader
             */
            std::shared_ptr<spdlog::logger> logger;
            SimulationData simData;
    };
} // namespace inputReader
