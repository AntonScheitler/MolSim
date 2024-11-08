#include "spdlogConfig.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "inputReader/TxtFileReader.h"
#include "particle/ParticleContainer.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <getopt.h>
#include "simulation/Simulator.h"

const std::string usageText =
        "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
        "-d, --delta_t\t\tsize of each timestep. defaults to 0.014\n"
        "-e, --t_end\t\ttime at which to stop the simulation. defaults to 1000\n"
        "-l, --log\t\tlog level, default value: 'info'. valid values (high to low):\n\t\t\t\t'trace', 'debug', 'info', 'warn', 'err', 'critical', 'off'\n\t\t\t\t (using any other string will result in logging turned 'off')\n"
        "-s, --sim_type\t\ttype of simulation to run:\n\t\t\t\t0 - Planets\n\t\t\t\t1 - Collision. Defaults to 0";

simTypes simType = simTypes::collision;
constexpr double startTime = 0;
double endTime = 1000;
double deltaT = 0.014;
double averageVelocity = 0.1;
spdlog::level::level_enum level;

ParticleContainer particles;

int main(int argc, char *argsv[]) {
    // input/options handling
    int c;
    const char* const shortOpts = "d:e:l:s:h";
    const option longOpts[] = {
        {"t_end", required_argument, nullptr, 'e'},
        {"delta_t", required_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'h'},
        {"sim_type", required_argument, nullptr, 's'},
        {"log", required_argument, nullptr, 'l'},
        {nullptr, no_argument, nullptr, 0}
   };

    // default logging level
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(spdlog::level::info);

    auto logger = spdlog::stdout_color_mt("MolSim");
    int simTypeNum;
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

    SPDLOG_LOGGER_INFO(logger, "MolSim program started with delta_t={0} and end_time={1}", deltaT, endTime);
    inputReader::TxtFileReader fileReader;
    fileReader.readFile(particles, argsv[optind]);
    Simulator simulator{comet, startTime, endTime, deltaT, particles};
    simulator.simulate();
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
    return 0;
}

