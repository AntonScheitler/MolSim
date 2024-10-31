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
        "-d, --delta_t\t\tsize of each timestep in seconds. defaults to 0.014\n"
        "-e, --t_end\t\ttime in seconds at which to stop the simulation. defaults to 1000";

constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

ParticleContainer particles;

int main(int argc, char *argsv[]) {
    // input/options handling
    int c;
    const char* const shortOpts = "d:e:h";
    const option longOpts[] = {
        {"t_end", required_argument, nullptr, 'e'},
        {"delta_t", required_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'h'},
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

    // read input from .txt file
    inputReader::TxtFileReader fileReader;
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
            outputWriter::VTKWriter writer;
            writer.plotParticles(particles, iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;
        current_time += delta_t;
    }
    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

