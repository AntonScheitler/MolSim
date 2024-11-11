#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "outputWriter/VTKWriter.h"


Simulator::Simulator(SimulationData& simDataArg) {
    simData = simDataArg;

    // choose computation functions based on the simType
    switch (simData.getSimType()) {
        // use gravity for the comet simulation
        case comet: 
            positionCompute = PositionComputations::stoermerVerlet;
            forceCompute = ForceComputations::computeGravity;
            velocityCompute = VelocityComputations::stoermerVerlet;
            logger = spdlog::stdout_color_mt("CometSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating planets and Halley's Comet");
            break;
        // use lennard-jones for the molecule collision
        case collision: 
            positionCompute = PositionComputations::stoermerVerlet;
            forceCompute = ForceComputations::computeLennardJonesPotential;
            velocityCompute = VelocityComputations::stoermerVerlet;
            logger = spdlog::stdout_color_mt("CollisionSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating 2-body collision");
            // initialize velocity via brownian motion
            VelocityComputations::applyBrownianMotion(simData.getParticles(), simData.getAverageVelocity());
            break;
    }
}

void Simulator::simulate() {
    // prepare for iteration
    double currentTime = simData.getStartTime();
    int iteration = 0;
    outputWriter::VTKWriter writer;

    // compute position, force and velocity for all particles each iteration
    while (currentTime < simData.getEndTime()) {
        ForceComputations::resetForces(simData.getParticles());
        positionCompute(simData.getParticles(), simData.getDeltaT());
        forceCompute(simData.getParticles());
        velocityCompute(simData.getParticles(), simData.getDeltaT());

        iteration++;
        if (iteration % 10 == 0) {
            // write output on every 10th iteration
            writer.plotParticles(simData.getParticles(), iteration);
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += simData.getDeltaT();
    }
}
