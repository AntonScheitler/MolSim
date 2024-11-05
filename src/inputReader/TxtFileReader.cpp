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
        int num_particles = 0;

        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open()) {

            getline(input_file, tmp_string);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);

            while (tmp_string.empty() or tmp_string[0] == '#') {
                getline(input_file, tmp_string);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);
            }

            std::istringstream numstream(tmp_string);
            numstream >> num_particles;
            SPDLOG_LOGGER_DEBUG(logger, "Reading {0}.", num_particles);
            getline(input_file, tmp_string);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);

            for (int i = 0; i < num_particles; i++) {
                std::istringstream datastream(tmp_string);

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

                getline(input_file, tmp_string);
                SPDLOG_LOGGER_DEBUG(logger, "Read line: {0}", tmp_string);
            }
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }
} // namespace inputReader
