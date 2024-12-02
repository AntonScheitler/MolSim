#include "computations/forces/ForceComputations.h"
#include "computations/positions/PositionComputations.h"
#include "computations/velocities/VelocityComputations.h"
#include "io/inputReader/TxtFileReader.h"
#include "simulation/Simulator.h"
#include "spdlogConfig.h"
#include <particle/container/ParticleContainerDirectSum.h>

int main(int argc, char *argsv[]) {
    // default logging level
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(spdlog::level::info);

    // setup data for the simulation
    SimulationData simData = SimulationData(); // TODO change this later
    int fileIdx = simData.parseOptions(argc, argsv);
    simData.readParticles(simData.getSimType(), argsv[fileIdx]);

    // perform the simulation/benchmark it
    Simulator simulator{simData};
    simulator.simulate();
    return 0;
}
