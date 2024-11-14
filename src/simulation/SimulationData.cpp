#include "particle/ParticleContainer.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <simulation/SimulationData.h>
#include <getopt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <inputReader/FileReader.h>


SimulationData::SimulationData() {
    // set default values
    simType = simulationType::collision;
    startTime = 0;
    endTime = 1000;
    deltaT = 0.014;
    epsilon = 5;
    sigma = 1;
    bench = false;
}

int SimulationData::parseOptions(int argc, char* argsv[]) {
    // define usage text
    std::string usageText = "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
        "-d, --delta_t\t\tsize of each timestep. Defaults to 0.014\n"
        "-e, --t_end\t\ttime at which to stop the simulation. Defaults to 1000\n"
        "-l, --log\t\tlog level, default value: 'info'. valid values (high to low):\n\t\t\t\t'trace', 'debug', 'info', 'warn', 'err', 'critical', 'off'\n\t\t\t\t (using any other string will result in logging turned 'off')\n"
        "-s, --sim_type\t\ttype of simulation to run:\n\t\t\t\t0 - Planets\n\t\t\t\t1 - Collision. Defaults to 1\n"
        "-b, --bench\t\tactivates benchmarking\n"
        "-S, --sigma\t\tinput sigma for Lennard-Jones potential. Defaults to 1\n"
        "-E, --epsilon\t\tinput epsilon for Lennard-Jones potential. Defaults to 5";

    // setup option parsing
    int c;
    int simTypeNum;
    const char* const shortOpts = "d:e:l:s:hbE:S:";
    const option longOpts[] = {
        {"delta_t", required_argument, nullptr, 'd'},
        {"t_end", required_argument, nullptr, 'e'},
        {"log", required_argument, nullptr, 'l'},
        {"sim_type", required_argument, nullptr, 's'},
        {"help", no_argument, nullptr, 'h'},
        {"bench", no_argument, nullptr, 'b'},
        {"epsilon", required_argument, nullptr, 'E'},
        {"sigma", required_argument, nullptr, 'S'},
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
                simType = simulationType(simTypeNum);
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
            case 'b':
                bench = true;
                break;
            case 'S':
                sigma = std::stod(optarg);
                if (sigma <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "sigma must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'E':
                epsilon = std::stod(optarg);
                if (epsilon <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "epsilon must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;

            case '?':
                SPDLOG_LOGGER_INFO(logger, usageText);
                exit(EXIT_FAILURE);
                break;
        }
    }
    // check if an input file has been supplied
    if (argc - optind != 1) {
        std::cerr << usageText << std::endl;
        exit(EXIT_FAILURE);
    }

    return optind;
}

void SimulationData::readParticles(simulationType, char* fileName) {
    inputReader::FileReader fileReader(*this);
    fileReader.readFile(particles, fileName);
}

simulationType SimulationData::getSimType() {
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

double SimulationData::getSigma() {
    return sigma;
}

double SimulationData::getEpsilon() {
    return epsilon;
}

bool SimulationData::getBench() {
    return bench;
}

ParticleContainer& SimulationData::getParticles() {
    return particles;
}

void SimulationData::setSimType(simulationType s) {
    this->simType = s;
}

void SimulationData::setParticlesCopy(ParticleContainer particlesArg) {
    particles = particlesArg;
}


