#include "outputWriter/VTKWriter.h"
#include "planets/StoermerVerlet.h"
#include <bits/getopt_core.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <unistd.h>
#include <vector>
#include "particle/ParticleContainer.h"
#include "forces/Forces.h"
#include "inputReaders/TxtFileReader.h"

// plots particles and writes the result to a vtk file
// @param interation the number of the current iteration
void plotParticles(int iteration);

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
  fileReader.readFile(particlesList, argsv[optind]);
  for (const Particle& planet : particlesList) {
    particles.push_back(planet);
  }

  double current_time = start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    // calculate new x
    computePositions(particles, delta_t);
    // calculate new f
    forces::computeGravity(particles);
    // calculate new v
    computeVelocities(particles, delta_t);

    iteration++;
    if (iteration % 10 == 0) {
      plotParticles(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  ParticleContainer pc{std::vector<Particle>{Particle{2}, Particle{6}}};

  for(const auto &p : pc) {
      std::cout << p.toString();
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void plotParticles(int iteration) {
  std::string out_name("MD_vtk");
  outputWriter::VTKWriter writer;
  writer.plotParticles(particles, out_name, iteration);
}
