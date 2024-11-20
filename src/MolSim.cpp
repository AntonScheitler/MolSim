#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "inputReader/TxtFileReader.h"
#include "simulation/Simulator.h"
#include "spdlogConfig.h"
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>

int main(int argc, char *argsv[]) {
    // default logging level
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(spdlog::level::info);

    // setup data for the simulation
    SimulationData simData = SimulationData();
    int fileIdx = simData.parseOptions(argc, argsv);
    simData.readParticles(simData.getSimType(), argsv[fileIdx]);

    // perform the simulation/benchmark it
    Simulator simulator{simData};
    if (simData.getBench()) {
        simulator.simulateBench();
    } else {
        simulator.simulate();
    }
    return 0;
}
