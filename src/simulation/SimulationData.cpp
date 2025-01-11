#include "particle/container/ParticleContainerDirectSum.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <simulation/SimulationData.h>
#include <getopt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <particle/container/ParticleContainer.h>
#include <io/inputReader/FileReader.h>


SimulationData::SimulationData() {
    // set default values
    particles = std::unique_ptr<ParticleContainer>(new ParticleContainerDirectSum());
    simType = SimulationType::collisionLinkedCell;
    startTime = 0;
    endTime = 5;
    deltaT = 0.0002;
    epsilon = 5;
    sigma = 1;
    bench = false;
    baseName = "MD_vtk";
    writeFrequency = 10;
    averageVelocity = 0;

    grav = 0;

    thermostat = false;

}

int SimulationData::parseOptions(int argc, char *argsv[]) {
    // define usage text
    std::string usageText = "Usage: ./MolSim [OPTIONS] INPUT_FILE\n"
                            "-d, --delta_t\t\tsize of each timestep. Defaults to 0.014\n"
                            "-e, --t_end\t\ttime at which to stop the simulation. Defaults to 1000\n"
                            "-l, --log\t\tlog level, default value: 'info'. valid values (high to low):\n\t\t\t\t'trace', 'debug', 'info', 'warn', 'err', 'critical', 'off'\n\t\t\t\t (using any other string will result in logging turned 'off')\n"
                            "-s, --sim_type\t\ttype of simulation to run:\n\t\t\t\t0 - Planets\n\t\t\t\t1 - Collision\n\t\t\t\t2 - Collision with linked cell\n\t\t\t3 - Membrane\n\t\t\t Defaults to 2\n"
                            "-b, --bench\t\tactivates benchmarking\n"
                            "-S, --sigma\t\tinput sigma for Lennard-Jones potential. Defaults to 1\n"
                            "-E, --epsilon\t\tinput epsilon for Lennard-Jones potential. Defaults to 5";

    // setup option parsing
    int c;
    int simTypeNum;
    const char *const shortOpts = "d:e:l:s:hbE:S:";
    const option longOpts[] = {
            {"delta_t",  required_argument, nullptr, 'd'},
            {"t_end",    required_argument, nullptr, 'e'},
            {"log",      required_argument, nullptr, 'l'},
            {"sim_type", required_argument, nullptr, 's'},
            {"help",     no_argument,       nullptr, 'h'},
            {"bench",    no_argument,       nullptr, 'b'},
            {"epsilon",  required_argument, nullptr, 'E'},
            {"sigma",    required_argument, nullptr, 'S'},
            {nullptr,    no_argument,       nullptr, 0}
    };

    auto logger = spdlog::stdout_color_mt("Parsing");

    while ((c = getopt_long(argc, argsv, shortOpts, longOpts, nullptr)) != -1) {
        switch (c) {
            case 'd':
                deltaT = std::stod(optarg);
                if (deltaT <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "delta t must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                endTime = std::stod(optarg);
                if (endTime <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "end time must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                simTypeNum = std::stoi(optarg);
                if (simTypeNum < 0 || simTypeNum > 3) {
                    SPDLOG_LOGGER_ERROR(logger, "simulation type must be between 0-3!");
                    exit(EXIT_FAILURE);
                }
                simType = SimulationType(simTypeNum);
                break;
            case 'h':
                SPDLOG_LOGGER_INFO(logger, usageText);
                exit(EXIT_SUCCESS);
            case 'l':
                level = spdlog::level::from_str(std::string(optarg));

                if (level >= 0 && level <= 6) {
                    spdlog::set_level(level);
                } else {
                    spdlog::set_level(spdlog::level::info);
                    SPDLOG_LOGGER_ERROR(logger, "Invalid log level.");
                    SPDLOG_LOGGER_INFO(logger, usageText);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'b':
                bench = true;
                break;
            case 'S':
                sigma = std::stod(optarg);
                if (sigma <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "sigma must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'E':
                epsilon = std::stod(optarg);
                if (epsilon <= 0) {
                    SPDLOG_LOGGER_ERROR(logger, "epsilon must be positive!");
                    exit(EXIT_FAILURE);
                }
                break;

            case '?':
                SPDLOG_LOGGER_INFO(logger, usageText);
                exit(EXIT_FAILURE);
                break;
        }
    }
    // check if an input file has been supplied
    if (argc - optind != 1) {
        std::cerr << usageText << std::endl;
        exit(EXIT_FAILURE);
    }

    return optind;
}

void SimulationData::readParticles(SimulationType, char *fileName) {
    inputReader::FileReader fileReader(*this);
    fileReader.readFile(fileName);
}

SimulationType SimulationData::getSimType() {
    return simType;
}

double SimulationData::getStartTime() {
    return startTime;
}

double SimulationData::getEndTime() {
    return endTime;
}

double SimulationData::getDeltaT() {
    return deltaT;
}

double SimulationData::getSigma() {
    return sigma;
}

double SimulationData::getEpsilon() {
    return epsilon;
}

bool SimulationData::getBench() {
    return bench;
}

ParticleContainer &SimulationData::getParticles() {
    return *particles;
}

void SimulationData::setParticles(std::unique_ptr<ParticleContainer> particlesArg) {
    particles = std::move(particlesArg);
}

void SimulationData::setSimType(SimulationType s) {
    this->simType = s;
}

void SimulationData::setParticlesCopy(ParticleContainer &particlesArg) {
    particles = particlesArg.copy();
}


void SimulationData::setEndTime(double endTime) {
    SimulationData::endTime = endTime;
}

void SimulationData::setStartTime(double startTime) {
    SimulationData::startTime = startTime;
}

void SimulationData::setDeltaT(double deltaT) {
    SimulationData::deltaT = deltaT;
}

void SimulationData::setEpsilon(double epsilon) {
    SimulationData::epsilon = epsilon;
}

void SimulationData::setSigma(double sigma) {
    SimulationData::sigma = sigma;
}

std::string SimulationData::getBaseName() {
    return baseName;
}

void SimulationData::setBaseName(const std::string &baseName) {
    SimulationData::baseName = baseName;
}

int SimulationData::getWriteFrequency() const {
    return writeFrequency;
}

void SimulationData::setWriteFrequency(int writeFrequency) {
    SimulationData::writeFrequency = writeFrequency;
}

void SimulationData::setAverageVelocity(double averageVelocityArg) {
    this->averageVelocity = averageVelocityArg;
}

double SimulationData::getAverageVelocity() {
    return averageVelocity;
}

double SimulationData::getInitialTemp() {
    return this->initialTemp;
}
double SimulationData::getMaxDeltaTemp() {
    return this->maxDeltaTemp;
}
double SimulationData::getTargetTemp() {
    return this->targetTemp;
}
size_t SimulationData::getThermoFrequency() {
    return this->thermoFrequency;
}

void SimulationData::activateThermostat() {
    this->thermostat = true;
}
void SimulationData::deactivateThermostat() {
    this->thermostat = false;
}
bool SimulationData::isThermostat() {
    return this->thermostat;
}

void SimulationData::setInitialTemp(double initialTempArg){
    this->initialTemp = initialTempArg;
}

void SimulationData::setMaxDeltaTemp(double maxDeltaTempArg) {
    this->maxDeltaTemp = maxDeltaTempArg;
}

void SimulationData::setTargetTemp(double targetTempArg) {
    this->targetTemp = targetTempArg;
}

void SimulationData::setThermoFrequency(size_t thermoFrequencyArg) {
    this->thermoFrequency = thermoFrequencyArg;
}

void SimulationData::setGrav(double gravArg) {
    this->grav = gravArg;
}

double SimulationData::getGrav(){
    return this->grav;
}

bool SimulationData::getCheckpoint() {
    return this->checkpoint;
}

void SimulationData::setCheckpoint(bool checkpoingArg) {
    this->checkpoint = checkpoingArg;
}

double SimulationData::getK() {
    return k;
}

void SimulationData::setK(double newK) {
    k = newK;
}

double SimulationData::getR0() {
    return r0;
}

void SimulationData::setR0(double newR0) {
    r0 = newR0;
}

std::vector<size_t> SimulationData::getMovingMembranePartIndices() {
    return movingMembranePartIndices;
}

void SimulationData::setMovingMembranePartIndices(std::vector<size_t> newMovingMembranePartIndices) {
    movingMembranePartIndices = newMovingMembranePartIndices;
}

std::array<double, 3> SimulationData::getCustomForce() {
    return customForce;
}

void SimulationData::setCustomForce(std::array<double, 3> newCustomForce) {
    customForce = newCustomForce;
}
