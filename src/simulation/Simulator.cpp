#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "io/outputWriter/VTKWriter.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "particle/container/ParticleContainer.h"
#include "particle/container/ParticleContainerLinkedCell.h"
#include <chrono>
#include <cstdlib>

Simulator::Simulator(SimulationData &simDataArg) : simData(simDataArg) {

    // choose computation functions based on the type
    switch (simData.getSimType()) {
        // use gravity for the comet simulation
        case comet:
            before = [this]() {};
            step = [this]() {
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::resetForces(simData.getParticles());
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
                VelocityComputations::applyBrownianMotion2D(simData.getParticles(), simData.getAverageVelocity());
            };
            step = [this]() {
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::resetForces(simData.getParticles());
                ForceComputations::computeLennardJonesPotential(simData.getParticles(), simData.getEpsilon(),
                                                                simData.getSigma());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());

            };
            after = [this]() {};
            logger = spdlog::stdout_color_mt("CollisionSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision");
            break;
        case collisionLinkedCell:
            before = [this]() {
                VelocityComputations::applyBrownianMotion2D(simData.getParticles(),
                                                            simData.getAverageVelocity());
            };
            step = [this]() {
                // save previous position and update the position of particles in the mesh based on the new one
                PositionComputations::updateOldX(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));
                if (containerLinkedCell) {
                    containerLinkedCell->correctAllParticleIndices();
                    ForceComputations::resetForces(simData.getParticles());
                    ForceComputations::computeLennardJonesPotentialCutoff(simData.getParticles(), simData.getEpsilon(),
                                                                    simData.getSigma(), containerLinkedCell->getCutoffRadius());
                    ForceComputations::computeGhostParticleRepulsion(*containerLinkedCell, simData.getEpsilon(),
                                                                     simData.getSigma());
                    VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                } else {
                    SPDLOG_ERROR("Linked Cell Simulation is not using Linked Cell Container. Aborting...");
                    exit(EXIT_FAILURE);
                }
            };
            after = [this]() {};
            logger = spdlog::stdout_color_mt("CollisionSimulationLinkedCell");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision with linked cell algorithm");
            break;
    }
}

void Simulator::simulate() {
    std::chrono::high_resolution_clock::rep totalDuration;
    int numIterations;

    if (simData.getBench()) {
        // overwrite logging settings
        spdlog::set_level(spdlog::level::info);
        // no macro logging statements, they are turned off while compiling when setting SPDLOG_ACTIVE_LEVEL to
        // SPDLOG_LEVEL_OFF in spdlogConfig.h => more performance for benchmark
        logger = spdlog::stdout_color_mt("Benchmarking");
        SPDLOG_LOGGER_INFO(logger, "=========================BENCH=========================");
        SPDLOG_LOGGER_INFO(logger, "Benchmarking with delta_t={0}, t_end={1}, sim_type={2}", simData.getDeltaT(),
                           simData.getEndTime(), (int) simData.getSimType());
        SPDLOG_LOGGER_INFO(logger, "Commencing Simulation...");

        totalDuration = 0;
        numIterations = 10;
    } else {
        SPDLOG_LOGGER_INFO(logger, "Starting Simulation with delta_t={0} and end_time={1}", simData.getDeltaT(),
                           simData.getEndTime());
    }

    if (simData.getBench()) {
        ParticleContainer &particlesBefore = simData.getParticles();
        for (int i = 0; i < numIterations; i++) {
            // turn off logging when benchmarking except for errors
            spdlog::set_level(spdlog::level::err);
            simData.setParticlesCopy(particlesBefore);
            auto start = std::chrono::high_resolution_clock::now();

            runSimulationLoop();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start);
            // turn logging back on to communicate results
            spdlog::set_level(spdlog::level::info);
            SPDLOG_LOGGER_INFO(logger, "Simulation no. {0} took {1} ms", i + 1, duration.count());
            totalDuration += duration.count();
        }
        SPDLOG_LOGGER_INFO(logger, "Simulation took {0} ms on average", (totalDuration / numIterations));
        SPDLOG_LOGGER_INFO(logger, "=========================BENCH=========================");
    } else {
        runSimulationLoop();
    }

}

void Simulator::runSimulationLoop() {
    // prepare for iteration
    double currentTime = simData.getStartTime();
    int iteration = 0;
    outputWriter::VTKWriter writer(simData.getBaseName());

    before();
    // compute position, force and velocity for all particles each iteration
    while (currentTime < simData.getEndTime()) {
        step();
        iteration++;

        if (iteration % simData.getWriteFrequency() == 0 && !simData.getBench()) {
            // write output on every ith iteration
            writer.plotParticles(simData.getParticles(), iteration);
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += simData.getDeltaT();
    }
    after();
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
}
