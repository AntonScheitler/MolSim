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
     * @brief set simulation type
     * @param startTime new start time
     */
    void setStartTime(double startTime);

    /**
     * @brief returns the start time of the simulation
     */
    double getStartTime();

    /**
     * @brief set simulation type
     * @param endTime new end time
     */
    void setEndTime(double endTime);

    /**
     * @brief returns the end time of the simulation
     */
    double getEndTime();

    /**
     * @brief set simulation type
     * @param deltaT new deltaT
     */
    void setDeltaT(double deltaT);

    /**
     * @brief returns the time step size of the simulation
     */
    double getDeltaT();


    /**
     * @brief set simulation type
     * @param epsilon new epsilon
     */
    void setEpsilon(double epsilon);

    /**
     * @brief returns the time step size of the simulation
     */
    double getEpsilon();

    /**
     * @brief set simulation type
     * @param sigma new sigma
     */
    void setSigma(double sigma);

    /**
     * @brief returns the time step size of the simulation
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
    //template <typename T>
    //typename std::enable_if<std::is_base_of<ParticleContainer, T>::value, void>::type

    void setParticlesCopy(ParticleContainer &particlesArg);

    void setParticles(std::unique_ptr<ParticleContainer> particles);

    std::string getBaseName();

    void setBaseName(const std::string &baseName);

    int getWriteFrequency() const;

    void setWriteFrequency(int writeFrequency);

    /**
     * @brief returns whether or not benchmarking is switched on
     */
    bool getBench();

    void setAverageVelocity(double averageVelocityArg);

    double getAverageVelocity();

    //thermo
    size_t getThermoFrequency();
    double getInitialTemp();
    double getTargetTemp();
    double getMaxDeltaTemp();


    void setThermoFrequency(size_t thermoFrequencyArg);
    void setInitialTemp(double initialTempArg);
    void setTargetTemp(double targetTempArg);
    void setMaxDeltaTemp(double maxDeltaTempArg);


    void activateThermostat();
    void deactivateThermostat();

    bool isThermostat();


    double getGrav();
    void setGrav(double gravArg);


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

};
