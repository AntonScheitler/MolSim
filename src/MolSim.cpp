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

    SPDLOG_LOGGER_INFO(logger, "MolSim program started with delta_t={0} and end_time={1}", deltaT, endTime);
    Simulator simulator{comet, startTime, endTime, deltaT, particles};
    simulator.simulate();
    SPDLOG_LOGGER_INFO(logger, "output written. Terminating...");
    return 0;
}

