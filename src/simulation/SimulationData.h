#pragma once

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <particle/ParticleContainer.h>

enum simulationType: int {
    comet = 0,
    collision = 1
};

class SimulationData {
    public:
        /**
         * @brief creates an instance of SimulationData which draws all information from commandline-arguments and files
         */
        SimulationData();
        /**
         * @brief parses command line options and sets class attributes accordingly 
         * @param argc the number of arguments passed to the program
         * @param argsv the arguments passed to the program
         * @return returns optind. So the index in argsv, where the unparsed arguments begin. This is usually the input file
         */
        int parseOptions(int argc, char* argsv[]);
        /**
         * @brief reads a file and adds particles to the SimulationData Object based on it's content
         * @param fileName the name of the file to read the particle information from
         */
        void readParticles(simulationType, char* fileName);

        /**
         * @brief set simulation type
         * @param s new sim type
         */
        void setSimType(simulationType s);
        /**
         * @brief returns the type of the simulation
         */
        simulationType getSimType();
        /**
         * @brief returns the start time of the simulation
         */
        double getStartTime();
        /**
         * @brief returns the end time of the simulation
         */
        double getEndTime();
        /**
         * @brief returns the time step size of the simulation
         */
        double getDeltaT();
        /**
         * @brief returns the particles to be simulated
         */
        ParticleContainer& getParticles();
    private:
        simulationType simType;
        double startTime;
        double endTime;
        double deltaT;
        spdlog::level::level_enum level;
        ParticleContainer particles;
};
