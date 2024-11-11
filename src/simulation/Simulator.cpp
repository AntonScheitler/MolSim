#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "outputWriter/VTKWriter.h"
#include <ctime>


void tmp () {
    int x = 5;
    int y = x - 2;
}

Simulator::Simulator(SimulationData& simDataArg) {
    simData = simDataArg;

    // choose computation functions based on the simType
    switch (simData.getSimType()) {
        // use gravity for the comet simulation
        case comet: 
            before = [this]() {};
            step = [this]() {
                ForceComputations::resetForces(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::computeGravity(simData.getParticles());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
            };
            after = [this]() {};
            logger = spdlog::stdout_color_mt("CometSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating planets and Halley's Comet");
            break;
            // use lennard-jones for the molecule collision
        case collision: 
            before = [this]() {
                VelocityComputations::applyBrownianMotion(simData.getParticles(), 
                        simData.getParticles().getAverageVelocity());
            };
            step = [this]() {
                ForceComputations::resetForces(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::computeLennardJonesPotential(simData.getParticles());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
            };
            after = [this]() {};
            logger = spdlog::stdout_color_mt("CollisionSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision");
            break;
    }
}


void Simulator::simulate() {
    // prepare for iteration
    double currentTime = simData.getStartTime();
    int iteration = 0;
    outputWriter::VTKWriter writer;
    before();

    // compute position, force and velocity for all particles each iteration
    while (currentTime < simData.getEndTime()) {
        step();

        iteration++;
        if (iteration % 10 == 0) {
            // write output on every 10th iteration
            writer.plotParticles(simData.getParticles(), iteration);
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += simData.getDeltaT();
    }
    after();
}
