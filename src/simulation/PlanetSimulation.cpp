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

namespace planetSimulation{

    void runPlanetSimulation( double start_time, double end_time, double delta_t, ParticleContainer &particles){




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

    }

}