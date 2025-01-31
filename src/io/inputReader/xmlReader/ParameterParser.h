#include "simulation/SimulationData.h"
#include "io/inputReader/xmlReader/xmlScheme/simulation.h"

namespace ParameterParser {
    /**
     * @brief reads the given elements from the parameter cluster of the xml file and stores them in the SimulationData Object
     * @param simData wrapper for data needed for the simulation
     * @param xmlParser pointer to xml-file
     */
    void readParams(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser);


    /**
     * @brief reads the given elements from the thermo cluster of the xml file and stores them in the SimulationData Object
     * @param simData wrapper for data needed for the simulation
     * @param xmlParser pointer to xml-file
     */
    void readThermo(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser);


    /**
     * @brief reads the given elements from the membrane cluster of the xml file and stores them in the SimulationData Object
     * @param simData wrapper for data needed for the simulation
     * @param xmlParser pointer to xml-file
     */
    void readMembrane(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser);

    /**
     * @brief reads the given elements from the parallelization cluster of the xml file and stores them in the SimulationData Object
     * @param simData wrapper for data needed for the simulation
     * @param xmlParser pointer to xml-file
     */
    void readParallelization(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser);

};



