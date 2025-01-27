#include "XMLFileReader.h"
#include "particle/boundary/Boundary.h"
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
#include "../CheckpointReader.h"
#include "spdlog/fmt/bundled/os.h"


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


        SPDLOG_LOGGER_DEBUG(logger, "entering XML parsing with filename {0}", filename);

        std::ifstream inputFile(filename);


        if (!inputFile) {
            throw std::runtime_error("Failed to open XML file.");
        }

        SPDLOG_LOGGER_DEBUG(logger, "opened input file");

        try {
            SPDLOG_LOGGER_DEBUG(logger, "opened simparser");


            std::unique_ptr<simulation> simParser = simulation_(inputFile, xml_schema::flags::dont_validate);
            SPDLOG_LOGGER_DEBUG(logger, "opened simparser");

            //            std::ostringstream command;
            //            command << "xmllint --noout --schema ../src/io/inputReader/xml/simulation.xsd " << filename;
            //            int result = std::system(command.str().c_str());

            //            if (!(result == 0 || result == 32512)) {
            //                std::cerr << "Error in xml file: scheme does not validate ";
            //                exit(-1);
            //            }

            // use linked cell if specified

            auto parameters = simParser->parameters();
            SPDLOG_LOGGER_DEBUG(logger, "entering Container Type");

            if (parameters.present() && parameters->containerType().present() &&
                parameters->containerType().get() == "linked") {
                auto containerLinkedCell = std::make_unique<ParticleContainerLinkedCell>(ParticleContainerLinkedCell{
                    {parameters->domainSize()->x(), parameters->domainSize()->y(), parameters->domainSize()->z()},
                    parameters->cutoff().get(), {


                        {getEnum(parameters->boundary()->yLeft()), getEnum(parameters->boundary()->yRight())},
                        {getEnum(parameters->boundary()->xBottom()), getEnum(parameters->boundary()->xTop())},
                        {
                            getEnum(parameters->boundary()->zFront()),
                            getEnum(parameters->boundary()->zBehind())
                        }
                    }
                });

                simData.setParticles(std::move(containerLinkedCell));
            } else {
                // use direct sum as default
                auto containerDirectSum = std::make_unique<ParticleContainerDirectSum>(ParticleContainerDirectSum{});
                simData.setParticles(std::move(containerDirectSum));
            }
            SPDLOG_LOGGER_DEBUG(logger, "finished reading container type");

            // read dimension number
            if (parameters.present() && parameters->domainSize().present()) {
                std::array<int, 3> dim = {
                    static_cast<int>(parameters->domainSize()->x()),
                    static_cast<int>(parameters->domainSize()->y()),
                    static_cast<int>(parameters->domainSize()->z())
                };
                if (dim[0] > 1 && dim[1] > 1 && dim[2] > 1) {
                    simData.setNumberDimensions(3);
                } else if (dim[0] == 1 || dim[1] == 1 || dim[2] == 1) {
                    simData.setNumberDimensions(2);
                } else
                    SPDLOG_LOGGER_WARN(logger, "Unable to specify number of dimensions precisely.");
            }
            SPDLOG_LOGGER_INFO(logger, "Number of dimensions set to: {0}", simData.getNumberDimensions());

            ParameterParser::readParams(simData, simParser);
            SPDLOG_LOGGER_DEBUG(logger, "finished reading params");

            ParameterParser::readThermo(simData, simParser);
            SPDLOG_LOGGER_DEBUG(logger, "finished reading thermo");

            ParameterParser::readMembrane(simData, simParser);
            SPDLOG_LOGGER_DEBUG(logger, "finished reading membrane");

            SPDLOG_LOGGER_DEBUG(logger, "read all params");

            if (simParser->parameters()->import_checkpoint().present()) {
                CheckpointReader checkpointReader(simData);
                type = checkpointReader.readCheckpointFile(simData,
                                                           simParser->parameters()->import_checkpoint()->file_path().
                                                           c_str());
            }

            SPDLOG_LOGGER_DEBUG(logger, "checkpointing done");

            type++;

            SPDLOG_LOGGER_DEBUG(logger, "starting parsing particles");

            for (const auto &planet: simParser->clusters().particle()) {
                x[0] = planet.coordinate().x();
                x[1] = planet.coordinate().y();
                x[2] = planet.coordinate().z();

                v[0] = planet.velocity().x();
                v[1] = planet.velocity().y();
                v[2] = planet.velocity().z();
                m = planet.mass();
                Particle temp = Particle(x, v, m);
                temp.setSigma(planet.sigma().present() ? planet.sigma().get() : simData.getSigma());
                temp.setEpsilon(planet.epsilon().present() ? planet.epsilon().get() : simData.getEpsilon());
                simData.getParticles().addParticle(temp);
                SPDLOG_LOGGER_DEBUG(logger, "adding particle at coords {0}, {1}, {2}", x[0], x[1], x[2]);
            }
            SPDLOG_LOGGER_INFO(logger, "finished reading particles");


            for (const auto &cuboid: simParser->clusters().cuboid()) {
                x[0] = cuboid.cornerCoordinates().x();
                SPDLOG_LOGGER_DEBUG(logger, "pos x: ", cuboid.cornerCoordinates().x());
                x[1] = cuboid.cornerCoordinates().y();
                SPDLOG_LOGGER_DEBUG(logger, "pos y: ", cuboid.cornerCoordinates().y());
                x[2] = cuboid.cornerCoordinates().z();
                SPDLOG_LOGGER_DEBUG(logger, "pos z: ", cuboid.cornerCoordinates().x());

                v[0] = cuboid.velocity().x();
                v[1] = cuboid.velocity().y();
                v[2] = cuboid.velocity().z();

                d[0] = cuboid.dimensions().x();
                d[1] = cuboid.dimensions().y();
                d[2] = cuboid.dimensions().z();

                m = cuboid.mass();
                h = cuboid.meshWidth();

                std::vector<size_t> movingMembranePartIndicesArgs;
                for (const auto &coords: cuboid.special_coords()) {
                    size_t index = coords.x() + coords.y() * d[0] + coords.z() * d[1] * d[0];
                    movingMembranePartIndicesArgs.push_back(index);
                }
                simData.setMovingMembranePartIndices(movingMembranePartIndicesArgs);


                simData.setAverageVelocity(cuboid.brownianMotion());

                double epsilon = cuboid.epsilon().present() ? cuboid.epsilon().get() : simData.getEpsilon();
                double sigma = cuboid.sigma().present() ? cuboid.sigma().get() : simData.getSigma();


                //TODO: also add 'fixed' param for each particle in checkpointing
                bool fixed = false;
                if(cuboid.fixed().present()) {
                    fixed = cuboid.fixed().get();
                }
                ParticleGenerator::generateCuboid(simData.getParticles(), x, v, d, m, h, type, epsilon, sigma, fixed);


                type++;
            }
            SPDLOG_LOGGER_INFO(logger, "finished reading cuboids");


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

                double epsilon = disc.epsilon().present() ? disc.epsilon().get() : simData.getEpsilon();
                double sigma = disc.sigma().present() ? disc.sigma().get() : simData.getSigma();

                bool fixed = false;
                if(disc.fixed().present()) {
                    fixed = disc.fixed().get();
                }
                ParticleGenerator::generateDisc(simData.getParticles(), x, v, r, m, h, type, epsilon, sigma, fixed);
                type++;
            }

            SPDLOG_LOGGER_INFO(logger, "finished reading discs ");
        } catch (const xml_schema::exception &e) {
            SPDLOG_LOGGER_ERROR(logger, "XML parsing error: {0}", e.what());
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            SPDLOG_ERROR("Standard exception: {0}", e.what());
            exit(EXIT_FAILURE);
        }
    }

    BoundaryType XMLFileReader::getEnum(const std::string &b) {
        if (b == "reflecting") {
            return reflect;
        } else if (b == "periodic") {
            return periodic;
        } else {
            return outflow;
        }
    }
}
