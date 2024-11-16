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

constexpr double startTime = 0;
double endTime = 1000;
double deltaT = 0.014;

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
                deltaT = std::stod(optarg);
                if (deltaT <= 0) {
                    std::cerr << "delta t must be positive!" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                endTime = std::stod(optarg);
                if (endTime <= 0) {
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
    double currentTime = startTime;
    int iteration = 0;

    // compute position, force and velocity for all particles each iteration
    while (currentTime < endTime) {
        positions::stoermerVerlet(particles, deltaT);
        forces::computeGravity(particles);
        velocities::stoermerVerlet(particles, deltaT);

        iteration++;
        if (iteration % 10 == 0) {
            // write output on every 10th iteration
            outputWriter::VTKWriter writer;
            writer.plotParticles(particles, iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;
        currentTime += deltaT;
    }
    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

