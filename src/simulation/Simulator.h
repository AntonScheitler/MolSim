#include "particle/ParticleContainer.h"

enum simTypes: int {
    comet = 0,
    collision = 1
};

/**
 * @brief a wrapper for all possible simulations.
 * automatically chooses the necessary computation methods based on the type of simulation desired
 */
class Simulator {
    public:
        Simulator(simTypes simType, double startTimeArg, double endTimeArg, 
                double deltaTArg, ParticleContainer& particlesArg, double averageVelocity);

        void simulate();
    private:
        double startTime;
        double endTime;
        double deltaT;
        ParticleContainer particles;

        // todo perform a single compute function
        void (*forceCompute)(ParticleContainer& particles);
        void (*positionCompute)(ParticleContainer& particles, double deltaT);
        void (*velocityCompute)(ParticleContainer& particles, double deltaT);

        void prepareSimulation();
};
