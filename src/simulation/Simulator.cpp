#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "computations/temperatures/TemperatureComputations.h"
#include "io/outputWriter/VTKWriter.h"
#include <io/outputWriter/CheckpointWriter.h>
#include "../io/outputWriter/VelocityDensityProfileWriter.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "particle/container/ParticleContainer.h"
#include "particle/container/ParticleContainerLinkedCell.h"
#include <chrono>
#include <cstdlib>
#include <string>

Simulator::Simulator(SimulationData &simDataArg) : simData(simDataArg) {
    // choose computation functions based on the type
    switch (simData.getSimType()) {
        // use gravity for the comet simulation
        case comet:
            before = [this]() {
            };
            step = [this](size_t iteration, double currentTime) {
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::resetForces(simData.getParticles());
                ForceComputations::computeGravity(simData.getParticles());
                ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
            };
            after = [this]() {
            };
            logger = spdlog::stdout_color_mt("CometSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating planets and Halley's Comet");
            break;
        // use lennard-jones for the molecule collision
        case collision:
            before = [this]() {
                VelocityComputations::applyBrownianMotion2D(simData.getParticles(), simData.getAverageVelocity());
            };
            step = [this](size_t iteration, double currentTime) {
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::resetForces(simData.getParticles());
                ForceComputations::computeLennardJonesPotential(simData.getParticles());
                ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
            };
            after = [this]() {
            };
            logger = spdlog::stdout_color_mt("CollisionSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision");
            break;
        case collisionLinkedCell:
            before = [this]() {
                if (simData.isThermostat()) {
                    TemperatureComputations::initTemp(simData.getParticles(), simData.getInitialTemp(),
                                                      simData.getNumberDimensions());
                } else {
                    VelocityComputations::applyBrownianMotion2D(simData.getParticles(),
                                                                simData.getAverageVelocity());
                }
            };
            step = [this](size_t iteration, double currentTime) {
                // save previous position and update the position of particles in the mesh based on the new one
                PositionComputations::updateOldX(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));
                if (containerLinkedCell) {
                    containerLinkedCell->correctCellMembershipAllParticles();
                    ForceComputations::resetForces(simData.getParticles());
                    ForceComputations::computeLennardJonesPotentialCutoffCellIter(*containerLinkedCell,
                                                                          containerLinkedCell->getCutoffRadius());


                    SPDLOG_DEBUG("computing ghost particle repulsion...");
                    ForceComputations::computeGhostParticleRepulsion(*containerLinkedCell);


                    ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());

                    VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());


                    if (simData.isThermostat() && iteration % simData.getThermoFrequency() == 0) {
                        // calculate current temperature of system
                        // TODO: choose correct thermostat (or use only v2 in future?)
                        if (simData.getThermoVersion() == 2) {
                            TemperatureComputations::updateTempV2(simData.getParticles(), simData.getTargetTemp(),
                                                                  simData.getMaxDeltaTemp(),
                                                                  simData.getNumberDimensions());
                        } else {
                            TemperatureComputations::updateTemp(simData.getParticles(), simData.getTargetTemp(),
                                                                simData.getMaxDeltaTemp(),
                                                                simData.getNumberDimensions());
                        }
                    }
                } else {
                    SPDLOG_ERROR("Linked Cell Simulation is not using Linked Cell Container. Aborting...");
                    exit(EXIT_FAILURE);
                }
            };
            after = [this]() {
            };
            logger = spdlog::stdout_color_mt("CollisionSimulationLinkedCell");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision with linked cell algorithm");
            break;
        case membrane:
            // TODO no brownian motion?
            before = [this]() {
            };
            step = [this](size_t iteration, double currentTime) {
                // save previous position and update the position of particles in the mesh based on the new one

                PositionComputations::updateOldX(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));
                if (containerLinkedCell) {
                    containerLinkedCell->correctCellMembershipAllParticles();

                    ForceComputations::resetForces(simData.getParticles());

                    ForceComputations::computeMembraneNeighborForce(*containerLinkedCell,
                                                                    simData.getK(), simData.getR0());

                    ForceComputations::computeGhostParticleRepulsion(*containerLinkedCell);

                    ForceComputations::addExternalForces(simData.getParticles(), simData.getGrav());

                    if (currentTime <= 150) {
                        ForceComputations::applyCustomForceVector(*containerLinkedCell,
                                                                  simData.getMovingMembranePartIndices(),
                                                                  simData.getCustomForce());
                    }

                    VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                } else {
                    SPDLOG_ERROR("Membrane simulation is not using Linked Cell Container. Aborting...");
                    exit(EXIT_FAILURE);
                }
            };
            after = [this]() {
            };
            logger = spdlog::stdout_color_mt("MembraneSimulationLinkedCell");
            SPDLOG_LOGGER_INFO(logger, "Simulating membrane with linked cell algorithm");
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
        numIterations = 10;
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

            double ms = duration.count();

            logger->info("Simulation no. {0} took {1} (total: {2} ms)", i + 1, formatTime(duration), ms);
            logger->info("{0} particles updated per second", (numUpdatedParticles * 1000.0) / ms);
            totalDuration += duration.count();
        }
        long avg = totalDuration / numIterations;
        auto avgTime = std::chrono::duration<long, std::ratio<1, 1000> >(avg);


        logger->info("Simulation took on average: {0} (total average: {1} ms)", formatTime(avgTime), avg);
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
    outputWriter::VelocityDensityProfileWriter profileWriter("profile");

    SPDLOG_LOGGER_INFO(logger, "delta_t={0}, t_end={1}, total number of iterations: {2}", simData.getDeltaT(), simData.getEndTime(),
                simData.getEndTime() / simData.getDeltaT());
    SPDLOG_LOGGER_INFO(logger, "Starting simulation...");

    before();
    SPDLOG_LOGGER_DEBUG(logger, "before step finished.");
    // compute position, force and velocity for all particles each iteration
    while (currentTime < simData.getEndTime()) {
        numUpdatedParticles += simData.getParticles().size();
        SPDLOG_LOGGER_DEBUG(logger, "before step instruction .");
        step(iteration, currentTime);
        SPDLOG_LOGGER_DEBUG(logger, "after step instruction.");
        iteration++;

        // do output file write in separate thread
        if (iteration % simData.getWriteFrequency() == 0 && !simData.getBench()) {
            writer.plotParticles(simData.getParticles(), iteration);
        }


        if (simData.getProfileIterationNumber() != -1 && iteration % simData.getProfileIterationNumber() == 0 && !simData.getBench()) {
            // write output on every 10th iteration
            try {
                auto& linkedCellContainer = dynamic_cast<ParticleContainerLinkedCell&>(simData.getParticles());
                profileWriter.profileBins(linkedCellContainer, iteration, simData.getProfileBinNumber());
            } catch (const std::bad_cast& e) {
                SPDLOG_LOGGER_ERROR(logger, "ParticleContainer is not of type ParticleContainerLinkedCell: ", e.what());
            }
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += simData.getDeltaT();
    }
    after();
    if (simData.getCheckpoint()) {
        outputWriter::CheckpointWriter checkpointWriter("checker");
        checkpointWriter.plotParticles(simData.getParticles(), 0);
    }
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
    return numUpdatedParticles;
}


Simulator::~Simulator() {
    spdlog::drop(logger->name());
}

std::string Simulator::formatTime(std::chrono::duration<long, std::ratio<1, 1000> > duration) {
    auto h = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= h;
    auto min = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= min;
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= sec;
    auto msLeft = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    return std::to_string(h.count()) + "h " + std::to_string(min.count()) + "min " + std::to_string(sec.count()) +
           "sec " + std::to_string(msLeft.count()) + "ms";
}
