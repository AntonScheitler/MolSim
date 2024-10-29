#include "computations/forces/Forces.h"
#include "computations/positions/Positions.h"
#include "computations/velocities/Velocities.h"
#include "inputReader/TxtFileReader.h"
#include "outputWriter/VTKWriter.h"
#include "particle/ParticleContainer.h"
#include <bits/getopt_core.h>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <unistd.h>
#include <vector>

const std::string usageText =
    "Usage: ./MolSim inputFile [-d delta_t] [-e t_end]\n"
    "-d: size of each timestep. defaults to 0.014\n"
    "-e time at which to stop the simulation. defaults to 1000";

constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

// TODO: what data structure to pick?
std::vector<Particle> particles;
std::list<Particle> particlesList;

int main(int argc, char *argsv[]) {
  // input/options handling
  int c;
  while ((c = getopt(argc, argsv, "d:e:h")) != -1) {
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

  inputReader::TxtFileReader fileReader;
  // todo rebuild list to vector?
  fileReader.readFile(particles, argsv[optind]);

  double current_time = start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    // calculate new x
    positions::stoermerVerlet(particles, delta_t);
    // calculate new f
    forces::computeGravity(particles);
    // calculate new v
    velocities::stoermerVerlet(particles, delta_t);

    iteration++;
    if (iteration % 10 == 0) {
      outputWriter::VTKWriter writer;
      writer.plotParticles(particles, iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  ParticleContainer pc{std::vector<Particle>{Particle{2}, Particle{6}}};

  for (const auto &p : pc) {
    std::cout << p.toString();
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}
