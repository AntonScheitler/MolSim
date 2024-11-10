#include "spdlogConfig.h"
#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "inputReader/TxtFileReader.h"
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include "simulation/Simulator.h"

int main(int argc, char *argsv[]) {
    // default logging level
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(spdlog::level::info);
    auto logger = spdlog::stdout_color_mt("MolSim");

    SimulationData simData = SimulationData();
    int fileIdx = simData.parseOptions(argc, argsv);
    simData.readParticles(simData.getSimType(), argsv[fileIdx]);

    SPDLOG_LOGGER_INFO(logger, "MolSim program started with delta_t={0} and end_time={1}", simData.getDeltaT(), simData.getEndTime());
    Simulator simulator{simData};
    simulator.simulate();
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
    return 0;
}
