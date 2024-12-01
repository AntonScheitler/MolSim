#include "ParameterParser.h"
#include "io/inputReader/xml/simulation.h"
#include "simulation/SimulationData.h"


namespace ParameterParser {

    void readParams(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser) {
        if (xmlParser->parameters().present()) {
            if (xmlParser->parameters()->delta_t().present()) {
                simData.setDeltaT(xmlParser->parameters()->delta_t().get());
            }
            if (xmlParser->parameters()->t_end().present()) {
                simData.setEndTime(xmlParser->parameters()->t_end().get());
            }
            if (xmlParser->parameters()->epsilon().present()) {
                simData.setEpsilon(xmlParser->parameters()->epsilon().get());
            }
            if (xmlParser->parameters()->sigma().present()) {
                simData.setSigma(xmlParser->parameters()->sigma().get());
            }
            if (xmlParser->parameters()->t_start().present()) {
                simData.setStartTime(xmlParser->parameters()->t_start().get());
            }
        }
        if(xmlParser->output().present()) {
            if (xmlParser->output()->baseName().present()) {
                simData.setBaseName(xmlParser->output()->baseName().get());
            }
            if (xmlParser->output()->writeFrequency().present()) {
                simData.setWriteFrequency(xmlParser->output()->writeFrequency().get());
            }

        }
    }
}