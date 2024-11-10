#include "JsonFileReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlogConfig.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace inputReader {

    JsonFileReader::JsonFileReader() {
        this->logger = spdlog::stdout_color_st("JsonFileReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized JsonFileReader");
    }

    void JsonFileReader::readCometFile(ParticleContainer &particles, char *filename) {
        std::array<double, 3> x{};
        std::array<double, 3> v{};

        double m;

        std::ifstream inputFile(filename);
        json data = json::parse(inputFile);

        if (inputFile.is_open()) {
            for (const auto& planet : data["planets"]) {

                x = cuboid["coordinates"].get<std::array<double, 3>>();
                v = cuboid["velocity"].get<std::array<double, 3>>();
                m = cuboid["mass"].get<double>();

                particles.addParticle(Particle(x, v, m));

            }
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }

    void JsonFileReader::readCollisionFile(ParticleContainer &particles, char *filename) {
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        std::array<int, 3> d{};
        double m;
        double h;
        double bm;

        std::ifstream inputFile(filename);
        json data = json::parse(inputFile);

        if (inputFile.is_open()) {
            for (const auto& cuboid : data["cuboids"]) {

                x = cuboid["cornerCoordinates"].get<std::array<double, 3>>();
                v = cuboid["velocity"].get<std::array<double, 3>>();
                d = cuboid["dimensions"].get<std::array<int, 3>>();
                m = cuboid["mass"].get<double>();
                h = cuboid["meshWidth"].get<double>();
                bm = cuboid["brownianMotion"].get<double>();

                std::array<double, 3> tempx{};
                for (int j = 1; j <= d[0]; ++j) {
                    tempx[0] = j * h + x[0];
                    for (int k = 1; k <= d[1]; ++k) {
                        tempx[1] = k * h + x[1];
                        for (int l = 1; l <= d[2]; ++l) {
                            tempx[2] = l * h + x[2];
                            particles.addParticle(Particle(tempx, v, m));
                        }
                    }
                }
            }
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }


} // namespace inputReader
