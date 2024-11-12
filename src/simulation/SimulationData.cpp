#include "particle/ParticleContainer.h"
#include <iostream>
#include <simulation/SimulationData.h>
#include <getopt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <inputReader/FileReader.h>


SimulationData::SimulationData() {
    // set default values
    simType = simTypes::collision;
    startTime = 0;
    endTime = 1000;
    deltaT = 0.014;
}

int SimulationData::parseOptions(int argc, char* argsv[]) {
    // define usage text
    std::string usageText = "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
        "-d, --delta_t\t\tsize of each timestep. defaults to 0.014\n"
        "-e, --t_end\t\ttime at which to stop the simulation. defaults to 1000\n"
        "-l, --log\t\tlog level, default value: 'info'. valid values (high to low):\n\t\t\t\t'trace', 'debug', 'info', 'warn', 'err', 'critical', 'off'\n\t\t\t\t (using any other string will result in logging turned 'off')\n"
        "-s, --sim_type\t\ttype of simulation to run:\n\t\t\t\t0 - Planets\n\t\t\t\t1 - Collision. Defaults to 1";

    // setup option parsing
    int c;
    int simTypeNum;
    const char* const shortOpts = "d:e:l:s:h";
    const option longOpts[] = {
        {"t_end", required_argument, nullptr, 'e'},
        {"delta_t", required_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'h'},
        {"sim_type", required_argument, nullptr, 's'},
        {"log", required_argument, nullptr, 'l'},
        {nullptr, no_argument, nullptr, 0}
    };

    auto logger = spdlog::stdout_color_mt("Parsing");

    while ((c = getopt_long(argc, argsv, shortOpts, longOpts, nullptr)) != -1) {
        switch (c) {
            case 'd':
                deltaT = std::stod(optarg);
                if (deltaT <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "delta t must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                endTime = std::stod(optarg);
                if (endTime <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "end time must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                simTypeNum = std::stoi(optarg);
                if (simTypeNum < 0 || simTypeNum > 1) {
                    SPDLOG_LOGGER_ERROR(logger, "simulation type must be between 0-1!");
                    exit(EXIT_FAILURE);
                }
                simType = simTypes(simTypeNum);
                break;
            case 'h':
                SPDLOG_LOGGER_INFO(logger, usageText);
                exit(EXIT_SUCCESS);
            case 'l':
                level = spdlog::level::from_str(std::string(optarg));

                if(level >= 0 && level <= 6) {
                    spdlog::set_level(level);
                } else {
                    spdlog::set_level(spdlog::level::info);
                    SPDLOG_LOGGER_ERROR(logger, "Invalid log level.");
                    SPDLOG_LOGGER_INFO(logger, usageText);
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                SPDLOG_LOGGER_INFO(logger, usageText);
                exit(EXIT_FAILURE);
        }
    }
    // check if an input file has been supplied
    if (argc - optind != 1) {
        std::cerr << usageText << std::endl;
        exit(EXIT_FAILURE);
    }

    return optind;
}

void SimulationData::readParticles(simTypes, char* fileName) {
    inputReader::FileReader fileReader(*this);
    fileReader.readFile(particles, fileName);
}

simTypes SimulationData::getSimType() {
    return simType;
}

double SimulationData::getStartTime() {
    return startTime;
}

double SimulationData::getEndTime() {
    return endTime;
}

double SimulationData::getDeltaT() {
    return deltaT;
}

ParticleContainer& SimulationData::getParticles() {
    return particles;
}
