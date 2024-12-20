#include "spdlogConfig.h"
#include <simulation/SimulationData.h>


namespace inputReader {
    /**
     * @brief a file reader specifically for txt files following a certain format
     */
    class CheckpointReader {
    public:
        /**
         * @brief creates an instance of the CheckpointReader
         */
        CheckpointReader(SimulationData &simulationDataArg);

        /**
         * @brief destructs an instance of the CheckpointReader
         */
        ~CheckpointReader();


        /**
         * @brief txt file reader specifically for the Collision Simulation
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        int readCheckpointFile(SimulationData &simData, const char *filename);

    private:

        /**
         * @brief a specific logger for the TxtFileReader
         */
        std::shared_ptr<spdlog::logger> logger;
        /**
         * @brief the simulation data to write the read information into
         */
        SimulationData &simData;
    };
} // namespace inputReader
