#include "ParameterParser.h"
#include "simulation/SimulationData.h"
#include <iostream>
#include "io/inputReader/xmlReader/xmlScheme/simulation.h"

/**
 * @brief namespace that wraps different methods to help parsing the XML input file
 */
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
                if (xmlParser->parameters()->averageVelocity().present()){
                    simData.setAverageVelocity(xmlParser->parameters()->averageVelocity().get());
                }
                if (xmlParser->parameters()->bin_profile().present()){
                    simData.setProfileIterationNumber(xmlParser->parameters()->bin_profile()->iteration());
                    if(xmlParser->parameters()->bin_profile()->bin_number().present()){
                        simData.setProfileBinNumber(xmlParser->parameters()->bin_profile()->bin_number().get());
                    }
                    if(xmlParser->parameters()->bin_profile()->base_name().present()){
                        simData.setProfilingBaseName(xmlParser->parameters()->bin_profile()->base_name().get());
                    }
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
            SPDLOG_ERROR("XML parsing error: {0}", e.what());
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            SPDLOG_ERROR("Standard exception: {0}", e.what());
            exit(EXIT_FAILURE);
        }
    }

    void readThermo(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser) {
        try {
            if (xmlParser->thermo().present()) {
                if(xmlParser->thermo()->version().present()) {
                    simData.setThermoVersion(xmlParser->thermo()->version().get());
                    SPDLOG_INFO("Using thermostat v{0}", simData.getThermoVersion());
                } else SPDLOG_INFO("thermostat is disabled");
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
            SPDLOG_ERROR("XML parsing error: {0}", e.what());
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            SPDLOG_ERROR("Standard exception: {0}", e.what());
            exit(EXIT_FAILURE);
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
            SPDLOG_ERROR("XML parsing error: {0}", e.what());
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            SPDLOG_ERROR("Standard exception: {0}", e.what());
            exit(EXIT_FAILURE);
        }
    }

    void readParallelization(SimulationData &simData, const std::unique_ptr<simulation> &xmlParser) {

        try {
            if (xmlParser->parallelization().present()) {
                simData.setThreadVersion(xmlParser->parallelization()->strategy());
                if (xmlParser->parallelization()->threadNumber().present()) {
                    simData.setNumberThreads(xmlParser->parallelization()->threadNumber().get());
                }
            }
        } catch (const xml_schema::exception &e) {
            SPDLOG_ERROR("XML parsing error: {0}", e.what());
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            SPDLOG_ERROR("Standard exception: {0}", e.what());
            exit(EXIT_FAILURE);
        }
    }
}
