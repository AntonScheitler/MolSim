#include "particle/ParticleContainer.h"
#include "spdlogConfig.h"
#include <simulation/SimulationData.h>


/**
 * @brief a wrapper for all possible simulations.
 * automatically chooses the necessary computation methods based on the type of simulation desired
 */
class Simulator {
    public:
        Simulator(SimulationData& simDataArg);
        void simulate();
    private:
        SimulationData simData;
        std::shared_ptr<spdlog::logger> logger;

        // todo perform a single compute function
        void (*forceCompute)(ParticleContainer& particles);
        void (*positionCompute)(ParticleContainer& particles, double deltaT);
        void (*velocityCompute)(ParticleContainer& particles, double deltaT);
};
