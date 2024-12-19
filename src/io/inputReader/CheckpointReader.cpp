#include "CheckpointReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlogConfig.h"



namespace inputReader {

    CheckpointReader::CheckpointReader(SimulationData &simulationDataArg) : simData(simulationDataArg) {
        this->logger = spdlog::stdout_color_st("CheckpointReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized CheckpointReader");
    }

    CheckpointReader::~CheckpointReader() {
        spdlog::drop("CheckpointReader");
    }


    int CheckpointReader::readCheckpointFile(SimulationData &simData, const char *filename) {
        int maxType = 0;

        std::array<double, 3> oldX{};
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        std::array<double, 3> f{};
        std::array<double, 3> oldF{};
        double m;
        int type;
        double epsilon;
        double sigma;

        int numParticles = 0;

        std::ifstream inputFile(filename);
        std::string tmpString;


        if (inputFile.is_open()) {

            getline(inputFile, tmpString);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);
            while (tmpString.empty() or tmpString[0] == '#') {
                getline(inputFile, tmpString);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);
            }

            std::istringstream numstream(tmpString);
            numstream >> numParticles;
            SPDLOG_LOGGER_DEBUG(logger, "Reading {0}.", numParticles);
            getline(inputFile, tmpString);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);

            for (int i = 0; i < numParticles; i++) {
                std::istringstream datastream(tmpString);

                for (auto &xj: oldX) {
                    datastream >> xj;
                }
                for (auto &xj: x) {
                    datastream >> xj;
                }
                for (auto &vj: v) {
                    datastream >> vj;
                }
                for (auto &fj: f) {
                    datastream >> fj;
                }
                for (auto &fj: oldF) {
                    datastream >> fj;
                }

                datastream >> m;

                datastream >> type;

                datastream >> epsilon;

                datastream >> sigma;

                if(maxType < type){
                    maxType = type;
                }
                Particle p = Particle(x, v, m);

                p.setOldX(oldX);
                p.setOldF(oldF);
                p.setF(f);
                p.setType(type);
                p.setEpsilon(epsilon);
                p.setSigma(sigma);

                simData.getParticles().addParticle(p);

                getline(inputFile, tmpString);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);
            }
            SPDLOG_LOGGER_DEBUG(logger, "Successfully read {0} particles", simData.getParticles().size());
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }


        return maxType;
    }
} // namespace inputReader
