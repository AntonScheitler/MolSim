#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "computations/temperatures/TemperatureComputations.h"
#include "io/outputWriter/VTKWriter.h"
#include <io/outputWriter/CheckpointWriter.h>
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
            step = [this](size_t iteration) {
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::resetForces(simData.getParticles());
                ForceComputations::computeGravity(simData.getParticles());
                ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());
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
            step = [this](size_t iteration) {
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::resetForces(simData.getParticles());
                ForceComputations::computeLennardJonesPotential(simData.getParticles(), simData.getEpsilon(),
                                                                simData.getSigma());
                ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());

            };
            after = [this]() {};
            logger = spdlog::stdout_color_mt("CollisionSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision");
            break;
        case collisionLinkedCell:
            before = [this]() {

                if (simData.isThermostat()) {
                    TemperatureComputations::initTemp(simData.getParticles(), simData.getAverageVelocity(),
                                                      simData.getInitialTemp());
                } else {
                    VelocityComputations::applyBrownianMotion2D(simData.getParticles(),
                                                                simData.getAverageVelocity());

                }
            };
            step = [this](size_t iteration) {
                // save previous position and update the position of particles in the mesh based on the new one
                PositionComputations::updateOldX(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));
                if (containerLinkedCell) {
                    containerLinkedCell->correctCellMembershipAllParticles();
                    ForceComputations::resetForces(simData.getParticles());
                    ForceComputations::computeLennardJonesPotentialCutoff(*containerLinkedCell, simData.getEpsilon(),
                                                                    simData.getSigma(), containerLinkedCell->getCutoffRadius());

                    SPDLOG_DEBUG("computing ghost particle repulsion...");
                    ForceComputations::computeGhostParticleRepulsion(*containerLinkedCell, simData.getEpsilon(),
                                                                     simData.getSigma());
                    // TODO: should this be here? was excluded in profiling branch
                    ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());
                    SPDLOG_DEBUG("done");
                    VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());


                    if (simData.isThermostat() && iteration % simData.getThermoFrequency() == 0) {
                        // calculate current temperature of system
                        TemperatureComputations::updateTemp(simData.getParticles(), simData.getTargetTemp(),
                                                            simData.getMaxDeltaTemp());

                    }

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
    size_t numIterations;

    if (simData.getBench()) {
        // overwrite logging settings
        spdlog::set_level(spdlog::level::info);
        /*
         * no macro logging statements, they are turned off while compiling when setting SPDLOG_ACTIVE_LEVEL to
         * SPDLOG_LEVEL_OFF in spdlogConfig.h => more performance for benchmark
         */
        logger = spdlog::stdout_color_mt("Benchmarking");
        logger->info("=========================BENCH=========================");
        logger->info("Benchmarking with delta_t={0}, t_end={1}, sim_type={2}", simData.getDeltaT(),
                           simData.getEndTime(), (int) simData.getSimType());
        logger->info("Commencing Simulation...");

        totalDuration = 0;
        numIterations = 1; //TODO: change this later back to 10 or set it dynamically as program option
    } else {
        SPDLOG_LOGGER_INFO(logger, "Starting Simulation with delta_t={0} and end_time={1}", simData.getDeltaT(),
                           simData.getEndTime());
    }

    if (simData.getBench()) {
        std::unique_ptr<ParticleContainer> particlesBefore = simData.getParticles().copy();
        for (int i = 0; i < numIterations; i++) {
            // turn off logging when benchmarking except for errors
            spdlog::set_level(spdlog::level::err);
            simData.setParticles(particlesBefore->copy());
            auto start = std::chrono::high_resolution_clock::now();

            size_t numUpdatedParticles = runSimulationLoop();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start);
            // turn logging back on to communicate results
            spdlog::set_level(spdlog::level::info);

            logger->info("Simulation no. {0} took {1} ms", i + 1, duration.count());
            logger->info("{0} particles updated per second", (numUpdatedParticles * 1000.0) / duration.count());
            totalDuration += duration.count();
        }
        logger->info("Simulation took {0} ms on average", (totalDuration / numIterations));
        logger->info("=========================BENCH=========================");
    } else {
        runSimulationLoop();
    }

}

size_t Simulator::runSimulationLoop() {
    // prepare for iteration
    size_t numUpdatedParticles = 0;
    double currentTime = simData.getStartTime();
    int iteration = 0;
    outputWriter::VTKWriter writer(simData.getBaseName());

    before();
    // compute position, force and velocity for all particles each iteration
    while (currentTime < simData.getEndTime()) {

        numUpdatedParticles += simData.getParticles().size();
        step(iteration);

        iteration++;

        if (iteration % simData.getWriteFrequency() == 0 && !simData.getBench()) {
            // write output on every 10th iteration
            writer.plotParticles(simData.getParticles(), iteration);
        }


        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += simData.getDeltaT();
    }
    after();
    if(simData.getCheckpoint()){
        outputWriter::CheckpointWriter checkpointWriter("checker");
        checkpointWriter.plotParticles(simData.getParticles(), 0);
    }
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
    return numUpdatedParticles;
}

Simulator::~Simulator() {
    spdlog::drop(logger->name());
}
