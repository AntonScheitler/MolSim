#include "spdlogConfig.h"

#include "computations/forces/Forces.h"
#include "computations/positions/Positions.h"
#include "computations/velocities/Velocities.h"
#include "inputReader/TxtFileReader.h"
#include "outputWriter/VTKWriter.h"
#include "particle/ParticleContainer.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <getopt.h>

const std::string usageText =
        "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
        "-d, --delta_t\t\tsize of each timestep. defaults to 0.014\n"
        "-e, --t_end\t\ttime at which to stop the simulation. defaults to 1000\n"
        "-l, --log\t\tlog level, default value: 'info'. valid values (high to low): 'trace', 'debug', 'info', 'warn',"
        " 'err', 'critical', 'off'.  "
        "(using any other string will result in logging turned 'off')";

constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;
spdlog::level::level_enum level;

ParticleContainer particles;

int main(int argc, char *argsv[]) {
    // input/options handling
    int c;
    const char* const shortOpts = "d:e:l:h";
    const option longOpts[] = {
        {"t_end", required_argument, nullptr, 'e'},
        {"delta_t", required_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'h'},
        {"log", required_argument, nullptr, 'l'},
        {nullptr, no_argument, nullptr, 0}
   };

    // default logging level
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(spdlog::level::info);

    auto logger = spdlog::stdout_color_mt("MolSim");

    while ((c = getopt_long(argc, argsv, shortOpts, longOpts, nullptr)) != -1) {
        switch (c) {
            case 'd':
                delta_t = std::stod(optarg);
                if (delta_t <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "delta t must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                end_time = std::stod(optarg);
                if (end_time <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "end time must be positive!");
                    exit(EXIT_FAILURE);
                }
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

    SPDLOG_LOGGER_INFO(logger, "MolSim program started with delta_t={0} and end_time={1}", delta_t, end_time);

    // read input from .txt file
    inputReader::TxtFileReader fileReader{};
    fileReader.readFile(particles, argsv[optind]);

    // prepare for iteration
    double current_time = start_time;
    int iteration = 0;

    // compute position, force and velocity for all particles each iteration
    while (current_time < end_time) {
        positions::stoermerVerlet(particles, delta_t);
        forces::computeGravity(particles);
        velocities::stoermerVerlet(particles, delta_t);

        iteration++;
        if (iteration % 10 == 0) {
            // write output on every 10th iteration
            outputWriter::VTKWriter writer{};
            writer.plotParticles(particles, iteration);
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        current_time += delta_t;
    }
    SPDLOG_LOGGER_INFO(logger, "Output written. Terminating...");
    return 0;
}

