#include "FileReader.h"
#include "particle/ParticleContainer.h"
#include "spdlogConfig.h"

namespace inputReader {
    /**
     * @brief a file reader specifically for txt files following a certain format
     */
    class TxtFileReader{
        /**
         * @brief reads particle information from a .txt file and stores it in a list
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        public:
            TxtFileReader();
            ~TxtFileReader();
            /**
             * @brief json file reader specifically for the Comet Simulation
             */
            void readCometFile(ParticleContainer &particles, char *filename) ;
            /**
             * @brief json file reader specifically for the Collision Simulation
             */
            void readCollisionFile(ParticleContainer &particles, char *filename) ;
        private:
            std::shared_ptr<spdlog::logger> logger;
    };
} // namespace inputReader
