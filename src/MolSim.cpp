#include "FileReader.h"
#include "planets/Gravity.h"
#include "planets/StoermerVerlet.h"
#include "outputWriter/VTKWriter.h"
#include <iostream>
#include <list>
#include <vector>

/**** forward declaration of the calculation functions ****/

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

// TODO: what data structure to pick?
std::vector<Particle> particles;
std::list<Particle> particlesList;

int main(int argc, char *argsv[]) {

    // todo proper input handling
    // todo include t_end and delta_t
  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 2) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "./molsym filename" << std::endl;
  }

  FileReader fileReader;
  // todo rebuild list to vector?
  fileReader.readFile(particlesList, argsv[1]);
  for (Particle planet : particlesList) {
    particles.push_back(planet);
  }

  double current_time = start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    // calculate new x
    computePositions(particles, delta_t);
    // calculate new f
    computeGravity(particles);
    // calculate new v
    computeVelocities(particles, delta_t);

    iteration++;
    if (iteration % 10 == 0) {
      plotParticles(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void plotParticles(int iteration) {
  std::string out_name("MD_vtk");

  outputWriter::VTKWriter writer;
  writer.initializeOutput(particles.size());
  for (Particle planet : particles) {
      writer.plotParticle(planet);
  }
  writer.writeFile(out_name, iteration);
}
