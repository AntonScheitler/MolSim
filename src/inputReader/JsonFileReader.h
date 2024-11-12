#include "FileReader.h"
#include "particle/ParticleContainer.h"
#include "spdlogConfig.h"


namespace inputReader {
    /**
     * @brief a file reader specifically for json files following a certain format
     */
    class JsonFileReader {
        /**
         * @brief reads particle information from a .json file and stores it in a list
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
    public:

        JsonFileReader();
        ~JsonFileReader();
        void readCollisionFile(ParticleContainer &particles, char *filename);
        void readCometFile(ParticleContainer &particles, char *filename);

    private:

        std::shared_ptr<spdlog::logger> logger;
    };
} // namespace inputReader