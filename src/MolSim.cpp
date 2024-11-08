#include "computations/forces/Forces.h"
#include "computations/positions/Positions.h"
#include "computations/velocities/Velocities.h"
#include "inputReader/TxtFileReader.h"
#include "outputWriter/VTKWriter.h"
#include "particle/ParticleContainer.h"
#include "simulation/PlanetSimulation.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <getopt.h>

const std::string usageText =
        "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
        "-d, --delta_t\t\tsize of each timestep in seconds. defaults to 0.014\n"
        "-e, --t_end\t\ttime in seconds at which to stop the simulation. defaults to 1000";

constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;
int simType = 0;

ParticleContainer particles;

int main(int argc, char *argsv[]) {
    // input/options handling
    int c;
    const char* const shortOpts = "d:e:h:s";
    const option longOpts[] = {
        {"t_end", required_argument, nullptr, 'e'},
        {"delta_t", required_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'h'},
        {"simType", required_argument, nullptr, 's'},
        {nullptr, no_argument, nullptr, 0}
   };
    while ((c = getopt_long(argc, argsv, shortOpts, longOpts, nullptr)) != -1) {
        switch (c) {
            case 'd':
                delta_t = std::stod(optarg);
                if (delta_t <= 0) {
                    std::cerr << "delta t must be positive!" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                end_time = std::stod(optarg);
                if (end_time <= 0) {
                    std::cerr << "end time must be positive!" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                simType = std::stoi(optarg);
                if (simType < 0 || simType > 1) {
                    std::cerr << "simulation type must be between 0-1!" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                std::cout << usageText << std::endl;
                exit(EXIT_SUCCESS);
            case '?':
                std::cerr << usageText << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    // check if an input file has been supplied
    if (argc - optind != 1) {
        std::cerr << usageText << std::endl;
        exit(EXIT_FAILURE);
    }

    switch (simType) {
        case 0: {
            // read input from .txt file
            inputReader::TxtFileReader fileReader;
            fileReader.readFile(particles, argsv[optind]);

            planetSimulation::runPlanetSimulation(start_time, end_time, delta_t, particles);

            break;
        }
        case 1: {
            // assignment 2
            std::cout << "in second case" << std::endl;
            break;
        }
    }


    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

