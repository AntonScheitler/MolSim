#include "simulation/SimulationData.h"
#include "io/inputReader/xml/simulation.h"

namespace ParameterParser {
    void readParams(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser);

    void readThermo(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser);
};



