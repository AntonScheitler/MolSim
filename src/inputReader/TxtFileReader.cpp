#include "TxtFileReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlogConfig.h"

namespace inputReader {

    TxtFileReader::TxtFileReader() {
        this->logger = spdlog::stdout_color_st("TxtFileReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized TxtFileReader");
    }
    TxtFileReader::~TxtFileReader() {
        spdlog::drop("TxtFileReader");
    }


    void TxtFileReader::readCometFile(ParticleContainer &particles, char *filename) {
        particles.setAverageVelocity(0);
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        double m;
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

                for (auto &xj : x) {
                    datastream >> xj;
                }
                for (auto &vj : v) {
                    datastream >> vj;
                }
                if (datastream.eof()) {
                    SPDLOG_LOGGER_ERROR(logger, "Error reading file: eof reached unexpectedly reading from line {0}");
                    exit(-1);
                }
                datastream >> m;
                particles.addParticle(Particle(x, v, m));
                getline(inputFile, tmpString);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);
            }
            SPDLOG_LOGGER_DEBUG(logger, "Successfully read {0} particles", particles.size());
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }

    void TxtFileReader::readCollisionFile(ParticleContainer &particles, char *filename) {
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        std::array<int, 3> d{};
        double m;
        double h;
        double bm;
        int numContainer = 0;
        int type = 0;

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
            numstream >> numContainer;
            SPDLOG_LOGGER_DEBUG(logger, "Reading {0}.", numContainer);
            getline(inputFile, tmpString);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);

            for (int i = 0; i < numContainer; i++) {
                std::istringstream datastream(tmpString);

                for (auto &xj : x) {
                    datastream >> xj;
                }
                for (auto &vj : v) {
                    datastream >> vj;
                }
                if (datastream.eof()) {
                    SPDLOG_LOGGER_ERROR(logger, "Error reading file: eof reached unexpectedly reading from line {0}");
                    exit(-1);
                }
                datastream >> m;
                datastream >> h;
                for (auto &dj : d) {
                    datastream >> dj;
                }
                datastream >> bm;
                particles.setAverageVelocity(bm);

                std::array<double, 3> tempx{};
                for (int j = 0; j < d[0]; ++j) {
                    tempx[0] = j * h + x[0];
                    for (int k = 0; k < d[1]; ++k) {
                        tempx[1] = k * h + x[1];
                        for (int l = 0; l < d[2]; ++l) {
                            tempx[2] = l * h + x[2];
                            SPDLOG_LOGGER_DEBUG(logger, "adding particle at coords {0}, {1}, {2}", tempx[0], tempx[1], tempx[2]);
                            particles.addParticle(Particle(tempx, v, m, type));
                        }
                    }
                }
                getline(inputFile, tmpString);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmpString);
                type++;
            }
            SPDLOG_LOGGER_DEBUG(logger, "Successfully read {0} particles", particles.size());
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }
} // namespace inputReader
