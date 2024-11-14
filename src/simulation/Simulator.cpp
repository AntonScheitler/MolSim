#include "Simulator.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "outputWriter/VTKWriter.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <chrono>
#include <ctime>

Simulator::Simulator(SimulationData& simDataArg) {
    simData = simDataArg;

    // choose computation functions based on the type
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
                VelocityComputations::applyBrownianMotion(simData.getParticles(), simData.getParticles().getAverageVelocity());
            };
            step = [this]() {
                ForceComputations::resetForces(simData.getParticles());
                PositionComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
                ForceComputations::computeLennardJonesPotential(simData.getParticles(), simData.getEpsilon(), simData.getSigma());
                VelocityComputations::stoermerVerlet(simData.getParticles(), simData.getDeltaT());
            };
            after = [this]() {};
            logger = spdlog::stdout_color_mt("CollisionSimulation");
            SPDLOG_LOGGER_INFO(logger, "Simulating body collision");
            break;
    }
}

void Simulator::simulate() {
    SPDLOG_LOGGER_INFO(logger, "Starting Simulation with delta_t={0} and end_time={1}", simData.getDeltaT(), simData.getEndTime());
    // prepare for iteration
    double currentTime = simData.getStartTime();
    int iteration = 0;
    outputWriter::VTKWriter writer;

    before();
    // compute position, force and velocity for all particles each iteration
    while (currentTime < simData.getEndTime()) {
        step();

        iteration++;
        if (iteration % 10 == 0 && !simData.getBench()) {
            // write output on every 10th iteration
            writer.plotParticles(simData.getParticles(), iteration);
        }
        SPDLOG_LOGGER_INFO(logger, "Iteration {0} finished.", iteration);
        currentTime += simData.getDeltaT();
    }
    after();
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
}

void Simulator::simulateBench() {
    // overwrite logging settings
    spdlog::set_level(spdlog::level::info);
    // no macro logging statements, they are turned off while compiling when setting SPDLOG_ACTIVE_LEVEL to
    // SPDLOG_LEVEL_OFF in spdlogConfig.h => more performance for benchmark
    logger = spdlog::stdout_color_mt("Benchmarking");
    logger->info("=========================BENCH=========================");
    logger->info("Benchmarking with delta_t={0}, t_end={1}, sim_type={2}", simData.getDeltaT(), simData.getEndTime(), (int) simData.getSimType());
    logger->info("Commencing Simulation...");

    std::chrono::high_resolution_clock::rep totalDuration = 0;
    int numIterations = 10;
    ParticleContainer particlesBefore = simData.getParticles();

    for (int i = 0; i < numIterations; i++) {
        // turn off logging before starting simulation (only log errors)
        spdlog::set_level(spdlog::level::err);
        simData.setParticlesCopy(particlesBefore);
        auto start = std::chrono::high_resolution_clock::now();

        double currentTime = simData.getStartTime();
        before();
        while (currentTime < simData.getEndTime()) {
            step();
            currentTime += simData.getDeltaT();
        }
        after();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
        // turn logging back on
        spdlog::set_level(spdlog::level::info);
        logger->info("Simulation no. {0} took {1} ms", i + 1, duration.count());
        totalDuration += duration.count();
    }

    logger->info("Simulation took {0} ms on average", (totalDuration / numIterations));
    logger->info("=========================BENCH=========================");
}
