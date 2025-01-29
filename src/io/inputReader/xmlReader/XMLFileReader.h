#include "simulation/SimulationData.h"
#include "spdlogConfig.h"
#include "particle/boundary/Boundary.h"

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
         * @brief xml file reader for all simulation types
         * @param filename the name of the file to read from
         */
        void readFile(char *filename);

    private:

        /**
         * @brief a specific logger for the XMLFileReader
         */
        std::shared_ptr<spdlog::logger> logger;

        /**
         * @brief the simulationData where the read information is saved to
         */
        SimulationData &simData;

        /**
         * @brief get BoundaryType enum value based of name
         * @param b name of type, either reflecting, periodic or outflowing
         * @return the BoundaryType enum value
         */
        static BoundaryType getEnum(const std::string &b);

    };
} // namespace inputReader
