#include "spdlogConfig.h"
#include <simulation/SimulationData.h>

#include "io/outputWriter/VTKWriter.h"


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
    Simulator(SimulationData &simDataArg);

    /**
     * @brief deconstructs this object and drops the associated logger
     */
    ~Simulator();

    /**
     * @brief runs the simulation
     */
    void simulate();

    /**
     * @brief pointer to a function to run before a simulation
     */
    std::function<void(void)> before;
    /**
     * @brief pointer to a function to run in every step of a simulation
     */
    std::function<void(size_t, double)> step;
    /**
     * @brief pointer to a function to run after a simulation
     */
    std::function<void(void)> after;

private:
    SimulationData &simData;
    std::shared_ptr<spdlog::logger> logger;

    /**
     * @brief runs the core simulation loop. This function gets called a number of times when benchmarking and just
     * once if benchmarking is disabled
     * @return the number of particles updated during the simulation
     */
    size_t runSimulationLoop();

};
