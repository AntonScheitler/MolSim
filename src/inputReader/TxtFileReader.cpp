#include "TxtFileReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlog/spdlog.h"

namespace inputReader {

    void TxtFileReader::readFile(ParticleContainer &particles, char *filename) {
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        double m;
        int num_particles = 0;

        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open()) {

            getline(input_file, tmp_string);
            spdlog::debug("Read line: {0}", tmp_string);

            while (tmp_string.empty() or tmp_string[0] == '#') {
                getline(input_file, tmp_string);
                spdlog::debug("Read line: {0}", tmp_string);
            }

            std::istringstream numstream(tmp_string);
            numstream >> num_particles;
            spdlog::debug("Reading {0}.", num_particles);
            getline(input_file, tmp_string);
            spdlog::debug("Read line: {0}", tmp_string);

            for (int i = 0; i < num_particles; i++) {
                std::istringstream datastream(tmp_string);

                for (auto &xj : x) {
                    datastream >> xj;
                }
                for (auto &vj : v) {
                    datastream >> vj;
                }
                if (datastream.eof()) {
                    spdlog::error("Error reading file: eof reached unexpectedly reading from line {0}");
                    exit(-1);
                }
                datastream >> m;
                particles.addParticle(Particle(x, v, m));

                getline(input_file, tmp_string);
                spdlog::debug("Read line: {0}", tmp_string);
            }
        } else {
            spdlog::error("Error: could not open file {0}", filename);
            exit(-1);
        }
    }
} // namespace inputReader
