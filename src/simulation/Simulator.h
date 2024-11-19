#include "spdlogConfig.h"
#include <simulation/SimulationData.h>


/**
 * @brief a wrapper for running all simulations.
 * automatically chooses the necessary computation methods based on the type of simulation desired
 */
class Simulator {
    public:
        /**
         * @brief creates an instance of the simulator class
         * @param simDataArg the simulation data to run on
         */
        Simulator(SimulationData& simDataArg);
        /**
         * @brief runs the simulation
         */
        void simulate();
        /**
         * @brief runs the simulation in benchmark mode. This means that no outputs are produced 
         */
        void simulateBench();
    private:
        SimulationData simData;
        std::shared_ptr<spdlog::logger> logger;

        /**
         * @brief pointer to a function to run before a simulation
         */
        std::function<void(void)> before;
        /**
         * @brief pointer to a function to run in every step of a simulation
         */
        std::function<void(void)> step;
        /**
         * @brief pointer to a function to run after a simulation
         */
        std::function<void(void)> after;
};