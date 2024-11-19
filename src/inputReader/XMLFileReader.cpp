#include "XMLFileReader.h"
#include "simulation/SimulationData.h"
#include <iostream>
#include <fstream>
#include "spdlogConfig.h"
#include "inputReader/simParser/simulation.h"
#include <array>
#include "ParticleGenerator.h"




namespace inputReader {
    XMLFileReader::XMLFileReader(SimulationData &simDataArg) : simData(simDataArg) {
        this->logger = spdlog::stdout_color_st("XMLFileReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized XMLFileReader");

    }

    XMLFileReader::~XMLFileReader() {
        spdlog::drop("XMLFileReader");
    }

    void XMLFileReader::readParams() {
        //Todo for refactoring

    }

    void XMLFileReader::readCometFile(ParticleContainer &particles, char *filename) {
        particles.setAverageVelocity(0);
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        std::array<int, 3> d{};
        double m;
        double h;
        double r;
        int type = 0;

        std::cout << "entering XML parsing with filename"<< filename << std::endl;

        std::ifstream inputFile(filename);
        if (!inputFile) {
            throw std::runtime_error("Failed to open XML file.");
        }
        try {

        std::unique_ptr<simulation> simParser = simulation_(inputFile, xml_schema::flags::dont_validate);



        if(simParser->parameters().present()) {
            if (simParser->parameters()->delta_t().present()) {
                simData.setDeltaT(simParser->parameters()->delta_t().get());
            }
            if (simParser->parameters()->t_end().present()) {
                simData.setEndTime(simParser->parameters()->t_end().get());
            }
            if (simParser->parameters()->epsilon().present()) {
                simData.setEpsilon(simParser->parameters()->epsilon().get());
            }
            if (simParser->parameters()->sigma().present()) {
                simData.setSigma(simParser->parameters()->sigma().get());
            }
            if (simParser->parameters()->t_start().present()) {
                simData.setStartTime(simParser->parameters()->t_start().get());
            }
        }
        std::cout << "start parsing cuboids" << std::endl;

        for (const auto& planet : simParser->clusters().particle()) {

            x[0] = planet.coordinate().x();
            x[1] = planet.coordinate().y();
            x[2] = planet.coordinate().z();

            v[0] = planet.velocity().x();
            v[1] = planet.velocity().y();
            v[2] = planet.velocity().z();
            m = planet.mass();

            particles.addParticle(Particle(x, v, m));
            SPDLOG_LOGGER_DEBUG(logger, "adding particle at coords {0}, {1}, {2}", x[0], x[1], x[2]);
        }

        for (const auto& cuboid : simParser->clusters().cuboid()) {
            x[0] = cuboid.cornerCoordinates().x();
            std::cout << "pos x: " << cuboid.cornerCoordinates().x() << std::endl;
            x[1] = cuboid.cornerCoordinates().y();
            std::cout << "pos y: " << cuboid.cornerCoordinates().y() << std::endl;
            x[2] = cuboid.cornerCoordinates().z();
            std::cout << "pos z: " << cuboid.cornerCoordinates().x() << std::endl;

            v[0] = cuboid.velocity().x();
            v[1] = cuboid.velocity().y();
            v[2] = cuboid.velocity().z();

            d[0] = cuboid.dimensions().x();
            d[1] = cuboid.dimensions().y();
            d[2] = cuboid.dimensions().z();

            m = cuboid.mass();
            h = cuboid.meshWidth();

            particles.setAverageVelocity(cuboid.brownianMotion());

            ParticleGenerator::generateCuboid(particles, x, v, d, m, h, type);

            type++;
        }


        for (const auto& disc : simParser->clusters().disc()) {
            x[0] = disc.center().x();
            x[1] = disc.center().y();
            x[2] = disc.center().z();

            v[0] = disc.velocity().x();
            v[1] = disc.velocity().y();
            v[2] = disc.velocity().z();

            m = disc.mass();
            h = disc.meshWidth();
            r = disc.radius();

            //Todo implement generateDisc
            ParticleGenerator::generateDisc(particles, x, v, r, m, h, type);

            type++;
        }

        } catch (const xml_schema::exception& e) {
            std::cerr << "XML parsing error: " << e.what() << std::endl;
            exit(-1);
        } catch (const std::exception& e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            exit(-1);
        }
    }
}

