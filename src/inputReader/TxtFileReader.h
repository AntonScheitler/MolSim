#include "FileReader.h"
#include <vector>

namespace inputReader {
    class TxtFileReader : public FileReader {
        /**
         * @brief reads particle information from a .txt file and stores it in a list
         * @param particles the particles to store the information in
         * @param filename the name of the file to read from
         */
        public:
            void readFile(std::vector<Particle> &particles, char *filename) override;
    };
} // namespace inputReader
