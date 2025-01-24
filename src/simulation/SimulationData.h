#pragma once

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <particle/container/ParticleContainer.h>

/**
 * @brief an enum which describes the type of simulation to run
 */
enum SimulationType : int {
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
    SimulationData();

    /**
     * @brief parses command line options and sets class attributes accordingly
     * @param argc the number of arguments passed to the program
     * @param argsv the arguments passed to the program
     * @return returns optind. So the index in argsv, where the unparsed arguments begin. This is usually the input file
     */
    int parseOptions(int argc, char *argsv[]);

    /**
     * @brief reads a file and adds particles to the SimulationData Object based on it's content
     * @param fileName the name of the file to read the particle information from
     */
    void readParticles(SimulationType, char *fileName);

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
     * @brief set start time of the simulation
     * @param startTime new start time
     */
    void setStartTime(double startTime);

    /**
     * @brief returns the start time of the simulation
     */
    double getStartTime();

    /**
     * @brief set end time of the simulation
     * @param endTime new end time
     */
    void setEndTime(double endTime);

    /**
     * @brief returns the end time of the simulation
     */
    double getEndTime();

    /**
     * @brief set delta t (time step size)
     * @param deltaT new deltaT
     */
    void setDeltaT(double deltaT);

    /**
     * @brief returns the time step size of the simulation
     */
    double getDeltaT();

    /**
     * @brief set epsilon
     * @param epsilon new epsilon
     */
    void setEpsilon(double epsilon);

    /**
     * @brief returns epsilon of the simulation
     */
    double getEpsilon();

    /**
     * @brief set sigma
     * @param sigma new sigma
     */
    void setSigma(double sigma);

    /**
     * @brief returns sigma of the simulation
     */
    double getSigma();

    /**
     * @brief returns the particles to be simulated
     */
    ParticleContainer &getParticles();

    /**
     * @brief sets particles to a copy of particlesArg
     * @param particlesArg the particles to copy
     */
    void setParticlesCopy(ParticleContainer &particlesArg);

    /**
     * @brief sets the particles of this simulation
     * @param particles
     */
    void setParticles(std::unique_ptr<ParticleContainer> particles);

    /**
     * @brief returns the base name of this simulationData
     */
    std::string getBaseName();

    /**
     * @brief sets the base name of this simulationData
     * @param baseName new base name
     */
    void setBaseName(const std::string &baseName);

    /**
     * @brief returns the write frequency, i.e. the number of iterations after which an output file is created
     */
    int getWriteFrequency() const;

    /**
     * @brief sets the output file write frequency
     * @param writeFrequency the new write frequency
     */
    void setWriteFrequency(int writeFrequency);

    /**
     * @brief returns whether or not benchmarking is switched on
     */
    bool getBench();

    /**
     * @brief sets the average velocity
     * @param averageVelocityArg new average velocity
     */
    void setAverageVelocity(double averageVelocityArg);

    /**
     * @brief returns the average velocity
     */
    double getAverageVelocity();

    //thermo
    /**
     * @brief returns the frequency at which the thermostat is applied (after which number of iterations)
     */
    size_t getThermoFrequency();

    /**
     * @brief returns the initial temperature of the system
     */
    double getInitialTemp();

    /**
     * @brief returns the target temperature of the system
     */
    double getTargetTemp();

    /**
     * @brief returns the maximum temperature difference in one application of the thermostat
     */
    double getMaxDeltaTemp();

    /**
     * @brief sets the thermostat frequency
     * @param thermoFrequencyArg new thermostat frequency
     */
    void setThermoFrequency(size_t thermoFrequencyArg);

    /**
     * @brief sets the initial temperature
     * @param initialTempArg
     */
    void setInitialTemp(double initialTempArg);

    /**
     * @brief sets the target temperature of the system
     * @param targetTempArg the new target temperature
     */
    void setTargetTemp(double targetTempArg);

    /**
     * @brief sets the maximum temperature difference of this system
     * @param maxDeltaTempArg new maximum delta t
     */
    void setMaxDeltaTemp(double maxDeltaTempArg);

    /**
     * @brief activates the thermostat (i.e. thermostat gets applied in thermoFrequency to the system)
     */
    void activateThermostat();

    /**
     * @brief deactivates the thermostat
     */
    void deactivateThermostat();

    /**
     * @brief returns true if the thermostat as activated (with activateThermostat())
     */
    bool isThermostat();

    /**
     * @brief returns the gravitational acceleration
     */
    double getGrav();

    /**
     * @brief sets the gravitational acceleration
     * @param gravArg new gravity
     */
    void setGrav(double gravArg);

    /**
     * @brief returns true if checkpoint is activated
     */
    bool getCheckpoint();

    /**
     * @brief sets whether checkpointing is activated
     * @param checkpoingArg if checkpointing should be activated
     */
    void setCheckpoint(bool checkpoingArg);

private:
    SimulationType simType;
    double startTime;
    double endTime;
    double deltaT;
    double sigma;
    double epsilon;
    bool bench;
    int writeFrequency;
    std::string baseName;
    spdlog::level::level_enum level;
    std::unique_ptr<ParticleContainer> particles;
    double averageVelocity;
    // thermo
    size_t thermoFrequency;
    double initialTemp;
    double targetTemp;
    double maxDeltaTemp;
    bool thermostat;

    double grav;
    bool checkpoint;

};
