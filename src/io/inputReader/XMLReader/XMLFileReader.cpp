#include "XMLFileReader.h"
#include "simulation/SimulationData.h"
#include <iostream>
#include <fstream>
#include "spdlogConfig.h"
#include "io/inputReader/xml/simulation.h"
#include <array>
#include "io/inputReader/ParticleGenerator.h"
#include "ParameterParser.h"
#include <memory>
#include <particle/container/ParticleContainerLinkedCell.h>
#include <particle/container/ParticleContainerDirectSum.h>


namespace inputReader {
    XMLFileReader::XMLFileReader(SimulationData &simDataArg) : simData(simDataArg) {
        this->logger = spdlog::stdout_color_st("XMLFileReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized XMLFileReader");
    }

    XMLFileReader::~XMLFileReader() {
        spdlog::drop("XMLFileReader");
    }


    void XMLFileReader::readFile(char *filename) {
        simData.setAverageVelocity(0);
        std::array<double, 3> x{};
        std::array<double, 3> v{};
        std::array<int, 3> d{};
        double m;
        double h;
        double r;
        int type = 0;


        std::cout << "entering XML parsing with filename" << filename << std::endl;

        std::ifstream inputFile(filename);
        if (!inputFile) {
            throw std::runtime_error("Failed to open XML file.");
        }
        try {

//        std::unique_ptr<simulation> simParser = simulation_(inputFile, xml_schema::flags::dont_validate);
            std::unique_ptr<simulation> simParser = simulation_(inputFile, xml_schema::flags::dont_validate);

            // use linked cell if specified
            auto parameters = simParser->parameters();
            if (parameters.present() && parameters->containerType().present() &&
                parameters->containerType().get() == "linked") {
                auto containerLinkedCell = std::unique_ptr<ParticleContainer>(new ParticleContainerLinkedCell(
                        {parameters->domainSize()->x(), parameters->domainSize()->y(), parameters->domainSize()->z()},
                        parameters->cutoff().get(), {
                            {getEnum(parameters->boundary()->xBottom().get()), getEnum(parameters->boundary()->xTop().get())},
                            {getEnum(parameters->boundary()->yLeft().get()), getEnum(parameters->boundary()->yRight().get())},
                            {outflow, outflow}}));
                simData.setParticles(std::move(containerLinkedCell));
            } else {
                // use direct sum as default
                auto containerDirectSum = std::unique_ptr<ParticleContainer>(new ParticleContainerDirectSum());
                simData.setParticles(std::move(containerDirectSum));
            }
            ParameterParser::readParams(simData, simParser);

            SPDLOG_LOGGER_INFO(logger, "starting parsing cuboids");

            for (const auto &planet: simParser->clusters().particle()) {

                x[0] = planet.coordinate().x();
                x[1] = planet.coordinate().y();
                x[2] = planet.coordinate().z();

                v[0] = planet.velocity().x();
                v[1] = planet.velocity().y();
                v[2] = planet.velocity().z();
                m = planet.mass();

                simData.getParticles().addParticle(Particle(x, v, m));
                SPDLOG_LOGGER_DEBUG(logger, "adding particle at coords {0}, {1}, {2}", x[0], x[1], x[2]);
            }

            for (const auto &cuboid: simParser->clusters().cuboid()) {
                x[0] = cuboid.cornerCoordinates().x();
                SPDLOG_LOGGER_DEBUG("pos x: ", cuboid.cornerCoordinates().x());
                x[1] = cuboid.cornerCoordinates().y();
                SPDLOG_LOGGER_DEBUG("pos y: ", cuboid.cornerCoordinates().y());
                x[2] = cuboid.cornerCoordinates().z();
                SPDLOG_LOGGER_DEBUG("pos z: ", cuboid.cornerCoordinates().x());

                v[0] = cuboid.velocity().x();
                v[1] = cuboid.velocity().y();
                v[2] = cuboid.velocity().z();

                d[0] = cuboid.dimensions().x();
                d[1] = cuboid.dimensions().y();
                d[2] = cuboid.dimensions().z();

                m = cuboid.mass();
                h = cuboid.meshWidth();

                simData.setAverageVelocity(cuboid.brownianMotion());

                ParticleGenerator::generateCuboid(simData.getParticles(), x, v, d, m, h, type);

                type++;
            }


            for (const auto &disc: simParser->clusters().disc()) {
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
                ParticleGenerator::generateDisc(simData.getParticles(), x, v, r, m, h, type);
                type++;
            }

        } catch (const xml_schema::exception &e) {
            std::cerr << "XML parsing error: " << e.what() << std::endl;
            exit(-1);
        } catch (const std::exception &e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            exit(-1);
        }
    }
    BoundaryType XMLFileReader::getEnum(std::string b) {
        if (b == "reflecting") {
            return reflect;
        } else{
            return outflow;
        }
    }
}
