#include "ParameterParser.h"
#include "io/inputReader/xml/simulation.h"
#include "simulation/SimulationData.h"
#include <iostream>


namespace ParameterParser {

    void readParams(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser) {
        try {
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
                if (xmlParser->parameters()->grav().present()) {
                    auto grav = xmlParser->parameters()->grav();
                    simData.setGrav({grav->x(), grav->y(), grav->z()});
                }

            }
            if (xmlParser->output().present()) {
                if (xmlParser->output()->baseName().present()) {
                    simData.setBaseName(xmlParser->output()->baseName().get());
                }
                if (xmlParser->output()->writeFrequency().present()) {
                    simData.setWriteFrequency(xmlParser->output()->writeFrequency().get());
                }
                if (xmlParser->output()->create_checkpoint_file().present()) {
                    simData.setCheckpoint(xmlParser->output()->create_checkpoint_file().get());
                } else {
                    simData.setCheckpoint(false);
                }
            }
        } catch (const xml_schema::exception &e) {
            std::cerr << "XML parsing error: " << e.what() << std::endl;
            exit(-1);
        } catch (const std::exception &e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            exit(-1);
        }
    }

    void readThermo(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser) {
        try {
            if (xmlParser->thermo().present()) {
                simData.setThermoVersion(xmlParser->thermo()->version());
                simData.activateThermostat();
                simData.setInitialTemp(xmlParser->thermo()->init_T());
                simData.setThermoFrequency(xmlParser->thermo()->n());
                double targetTempArg = (xmlParser->thermo()->target().present()) ? (xmlParser->thermo()->target().get())
                                                                                 : xmlParser->thermo()->init_T();
                simData.setTargetTemp(targetTempArg);
                double maxStep = (xmlParser->thermo()->maxStep().present()) ? (xmlParser->thermo()->maxStep().get())
                                                                            : std::numeric_limits<double>::infinity();
                simData.setMaxDeltaTemp(maxStep);
            }
        } catch (const xml_schema::exception &e) {
            std::cerr << "XML parsing error: " << e.what() << std::endl;
            exit(-1);
        } catch (const std::exception &e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            exit(-1);
        }
    }

    void readMembrane(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser){

        try {
            if (xmlParser->membraneArgs().present()) {
                simData.setK(xmlParser->membraneArgs()->k());
                simData.setR0(xmlParser->membraneArgs()->r0());
                auto vector = xmlParser->membraneArgs()->customForce();
                simData.setCustomForce({vector.x(), vector.y(), vector.z()});
            }
        } catch (const xml_schema::exception &e) {
            std::cerr << "XML parsing error: " << e.what() << std::endl;
            exit(-1);
        } catch (const std::exception &e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            exit(-1);
        }
    }
}