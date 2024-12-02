#include "JsonFileReader.h"
#include <fstream>
#include "spdlogConfig.h"
#include "ParticleGenerator.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;

namespace inputReader {

    JsonFileReader::JsonFileReader(SimulationData &simDataArg) : simData(simDataArg) {
        this->logger = spdlog::stdout_color_st("JsonFileReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized JsonFileReader");
    }

    JsonFileReader::~JsonFileReader() {
        spdlog::drop("JsonFileReader");
    }

    void JsonFileReader::readFile(SimulationData &simData, char *filename) {
        simData.setAverageVelocity(0);
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        std::array<int, 3> d{};
        double m;
        double h;
        double bm;
        int type = 0;

        SPDLOG_LOGGER_DEBUG(logger, "Reading json file");

        std::ifstream inputFile(filename);
        json data = json::parse(inputFile);

        if (inputFile.is_open()) {
            for (const auto &planet: data["planets"]) {

                x = planet["coordinates"].get<std::array<double, 3>>();
                v = planet["velocity"].get<std::array<double, 3>>();
                m = planet["mass"].get<double>();

                simData.getParticles().addParticle(Particle{x, v, m});
                SPDLOG_LOGGER_DEBUG(logger, "adding particle at coords {0}, {1}, {2}", x[0], x[1], x[2]);
            }

            for (const auto &cuboid: data["cuboids"]) {
                x = cuboid["cornerCoordinates"].get<std::array<double, 3>>();
                v = cuboid["velocity"].get<std::array<double, 3>>();
                d = cuboid["dimensions"].get<std::array<int, 3>>();
                m = cuboid["mass"].get<double>();
                h = cuboid["meshWidth"].get<double>();
                bm = cuboid["brownianMotion"].get<double>();
                simData.setAverageVelocity(bm);

                ParticleGenerator::generateCuboid(simData.getParticles(), x, v, d, m, h, type);

                type++;

            }
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: could not open file {0}", filename);
            exit(-1);
        }
    }


} // namespace inputReader
