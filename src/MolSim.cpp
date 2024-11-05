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
#include "spdlog/spdlog.h"

const std::string usageText =
        "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
        "-d, --delta_t\t\tsize of each timestep in seconds. defaults to 0.014\n"
        "-e, --t_end\t\ttime in seconds at which to stop the simulation. defaults to 1000\n"
        "-l, --log\t\tlog level, valid values (high to low): trace, debug, info, warn, err, critical, off "
        "(using any other string will result in logging turned 'off'";

constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;
std::string level_str;
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

    spdlog::set_level(spdlog::level::info);

    while ((c = getopt_long(argc, argsv, shortOpts, longOpts, nullptr)) != -1) {
        switch (c) {
            case 'd':
                delta_t = std::stod(optarg);
                if (delta_t <= 0) {
                    spdlog::error("delta t must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                end_time = std::stod(optarg);
                if (end_time <= 0) {
                    spdlog::error("end time must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                spdlog::info(usageText);
                exit(EXIT_SUCCESS);
            case 'l':
                level_str = std::string(optarg);
                level = spdlog::level::from_str(level_str);
                std::cout << level << '\n';
                if(level >= 0 && level <= 6) {
                    spdlog::set_level(level);
                } else {
                    spdlog::set_level(spdlog::level::info);
                    spdlog::error("Invalid log level.");
                    spdlog::info(usageText);
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                spdlog::info(usageText);
                exit(EXIT_FAILURE);
        }
    }
    // check if an input file has been supplied
    if (argc - optind != 1) {
        std::cerr << usageText << std::endl;
        exit(EXIT_FAILURE);
    }


    // read input from .txt file
    inputReader::TxtFileReader fileReader;
    fileReader.readFile(particles, argsv[optind]);


    spdlog::info("MolSim program started with delta_t={0} and end_time={1}", delta_t, end_time);

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
            outputWriter::VTKWriter writer;
            writer.plotParticles(particles, iteration);
        }
        spdlog::info("Iteration {0} finished.", iteration);
        current_time += delta_t;
    }
    spdlog::info("Output written. Terminating...");
    return 0;
}

