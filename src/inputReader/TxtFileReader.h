#include "FileReader.h"
#include "particle/ParticleContainer.h"
#include "spdlogConfig.h"

namespace inputReader {
    /**
     * @brief a filereader specifically for txt files following a certain format
     */
    class TxtFileReader : public FileReader {
        /**
         * @brief reads particle information from a .txt file and stores it in a list
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        public:
            TxtFileReader();
            void readFile(ParticleContainer &particles, char *filename) override;
        private:
            std::shared_ptr<spdlog::logger> logger;
    };
} // namespace inputReader
