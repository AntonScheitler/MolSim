#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "inputReader/TxtFileReader.h"
#include "outputWriter/VTKWriter.h"
#include "particle/ParticleContainer.h"
#include "spdlogConfig.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>

namespace planetSimulation{

    void runPlanetSimulation( double startTime, double endTime, double deltaT, ParticleContainer &particles){
        // prepare for iteration
        double current_time = startTime;
        int iteration = 0;

        outputWriter::VTKWriter writer;
        // compute position, force and velocity for all particles each iteration
        while (current_time < endTime) {
            ForceComputations::resetForces(particles);
            PositionComputations::stoermerVerlet(particles, deltaT);
            ForceComputations::computeGravity(particles);
            VelocityComputations::stoermerVerlet(particles, deltaT);

            iteration++;
            if (iteration % 10 == 0) {
                // write output on every 10th iteration
                writer.plotParticles(particles, iteration);
            }
            SPDLOG_INFO("Iteration {0} finished.", iteration);
            current_time += deltaT;
        }
    }
}
