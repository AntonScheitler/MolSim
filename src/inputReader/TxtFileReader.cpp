#include "TxtFileReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace inputReader {

    void TxtFileReader::readFile(ParticleContainer &particles, char *filename) {
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        double m;
        int numParticles = 0;

        std::ifstream inputFile(filename);
        std::string tmpString;

        if (inputFile.is_open()) {

            getline(inputFile, tmpString);
            std::cout << "Read line: " << tmpString << std::endl;

            while (tmpString.empty() or tmpString[0] == '#') {
                getline(inputFile, tmpString);
                std::cout << "Read line: " << tmpString << std::endl;
            }

            std::istringstream numstream(tmpString);
            numstream >> numParticles;
            std::cout << "Reading " << numParticles << "." << std::endl;
            getline(inputFile, tmpString);
            std::cout << "Read line: " << tmpString << std::endl;

            for (int i = 0; i < numParticles; i++) {
                std::istringstream datastream(tmpString);

                for (auto &xj : x) {
                    datastream >> xj;
                }
                for (auto &vj : v) {
                    datastream >> vj;
                }
                if (datastream.eof()) {
                    std::cout
                        << "Error reading file: eof reached unexpectedly reading from line "
                        << i << std::endl;
                    exit(-1);
                }
                datastream >> m;
                particles.addParticle(Particle(x, v, m));

                getline(inputFile, tmpString);
                std::cout << "Read line: " << tmpString << std::endl;
            }
        } else {
            std::cout << "Error: could not open file " << filename << std::endl;
            exit(-1);
        }
    }
} // namespace inputReader
