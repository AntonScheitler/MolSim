#include "particle/ParticleContainerDirectSum.h"
#include "spdlogConfig.h"

namespace inputReader {
    /**
     * @brief a file reader specifically for txt files following a certain format
     */
    class TxtFileReader{
        public:
            /**
             * @brief creates an instance of the TxtFileReader
             */
            TxtFileReader();
            /**
             * @brief destructs an instance of the TxtFileReader
             */
            ~TxtFileReader();
            /**
             * @brief txt file reader specifically for the Comet Simulation
             * @param particles the particles to store the information in
             * @param filename the name of the file to read from
             */
            void readCometFile(ParticleContainer &particles, char *filename) ;
            /**
             * @brief txt file reader specifically for the Collision Simulation
             * @param particles the particles to store the information in
             * @param filename the name of the file to read from
             */
            void readCollisionFile(ParticleContainer &particles, char *filename) ;
        private:

            /**
             * @brief a specific logger for the TxtFileReader
             */
            std::shared_ptr<spdlog::logger> logger;
    };
} // namespace inputReader
