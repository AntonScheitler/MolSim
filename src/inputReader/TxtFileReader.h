#include "FileReader.h"
#include "particle/ParticleContainer.h"

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
            void readFile(ParticleContainer &particles, char *filename) override;
    };
} // namespace inputReader
