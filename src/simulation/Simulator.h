#include "spdlogConfig.h"
#include <functional>
#include <simulation/SimulationData.h>


/**
 * @brief a wrapper for all possible simulations.
 * automatically chooses the necessary computation methods based on the type of simulation desired
 */
class Simulator {
    public:
        Simulator(SimulationData& simDataArg);
       void simulate();
       void simulateBench();
    private:
        SimulationData simData;
        std::shared_ptr<spdlog::logger> logger;

        std::function<void(void)> before;
        std::function<void(void)> step;
        std::function<void(void)> after;
};
