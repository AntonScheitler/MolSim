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

    void TxtFileReader::readFile(ParticleContainer &particles, char *filename) {
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        double m;
        int numParticles = 0;

        std::ifstream inputFile(filename);
        std::string tmpString;

        if (inputFile.is_open()) {

            getline(inputFile, tmpString);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);

            while (tmpString.empty() or tmpString[0] == '#') {
                getline(inputFile, tmpString);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);
            }

            std::istringstream numstream(tmpString);
            numstream >> numParticles;
            getline(inputFile, tmpString);
            SPDLOG_LOGGER_DEBUG(logger, "Reading {0}.", numParticles);
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
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);
            }
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }
} // namespace inputReader
