#pragma once

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <particle/container/ParticleContainer.h>

/**
 * @brief an enum which describes the type of simulation to run
 */
enum SimulationType: int {
    comet = 0,
    collision = 1,
    collisionLinkedCell = 2
};

/**
 * @brief a class which wraps all simulation information from command line arguments and files
 */
class SimulationData {
    public:
        /**
         * @brief creates an instance of the SimulationData class
         */
        SimulationData(ParticleContainer& particlesArg);
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
        void readParticles(SimulationType, char* fileName);

        /**
         * @brief set simulation type
         * @param s new sim type
         */
        void setSimType(SimulationType s);
        /**
         * @brief returns the type of the simulation
         */
        SimulationType getSimType();
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
         * @brief returns the time step size of the simulation
         */
        double getEpsilon();
        /**
         * @brief returns the time step size of the simulation
         */
        double getSigma();
        /**
         * @brief returns the particles to be simulated
         */
        ParticleContainer& getParticles();

        /**
         * @brief sets particles to a copy of particlesArg
         * @param particlesArg the particles to copy
         */
        template <typename T>
        typename std::enable_if<std::is_base_of<ParticleContainer, T>::value, void>::type

         setParticlesCopy(const T& particlesArg);

        /**
         * @brief returns whether or not benchmarking is switched on
         */
        bool getBench();
    private:
        SimulationType simType;
        double startTime;
        double endTime;
        double deltaT;
        double sigma;
        double epsilon;
        bool bench;
        spdlog::level::level_enum level;
        ParticleContainer& particles;
};
