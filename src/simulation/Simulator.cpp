#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "outputWriter/VTKWriter.h"


Simulator::Simulator(simTypes simType, double startTimeArg, double endTimeArg, 
        double deltaTArg, ParticleContainer &particlesArg, double averageVelocity) {
    particles = particlesArg;
    startTime = startTimeArg;
    endTime = endTimeArg;
    deltaT = deltaTArg;

    // choose computation functions based on the simType
    switch (simType) {
        // use gravity for the comet simulation
        case comet: 
            positionCompute = PositionComputations::stoermerVerlet;
            forceCompute = ForceComputations::computeGravity;
            velocityCompute = VelocityComputations::stoermerVerlet;
            logger = spdlog::stdout_color_mt("CometSimulation");
            break;
        // use lennard-jones for the molecule collision
        case collision: 
            positionCompute = PositionComputations::stoermerVerlet;
            forceCompute = ForceComputations::computeLennardJonesPotential;
            velocityCompute = VelocityComputations::stoermerVerlet;
            logger = spdlog::stdout_color_mt("CollisionSimulation");

            // initialize velocity via brownian motion
            VelocityComputations::applyBrownianMotion(particles, averageVelocity);
            break;
    }
}

void Simulator::simulate() {
    // prepare for iteration
    double currentTime = startTime;
    int iteration = 0;
    outputWriter::VTKWriter writer;

    // compute position, force and velocity for all particles each iteration
    while (currentTime < endTime) {
        ForceComputations::resetForces(particles);
        positionCompute(particles, deltaT);
        forceCompute(particles);
        velocityCompute(particles, deltaT);

        iteration++;
        if (iteration % 10 == 0) {
            // write output on every 10th iteration
            writer.plotParticles(particles, iteration);
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += deltaT;
    }
}
