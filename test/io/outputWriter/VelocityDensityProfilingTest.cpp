#include <gtest/gtest.h>
#include <utils/ArrayUtils.h>
#include "../../../src/simulation/SimulationData.h"
#include "../../../src/particle/container/ParticleContainerLinkedCell.h"
#include "../../../src/io/outputWriter/VelocityDensityProfileWriter.h"
#include "../../../src/particle/Particle.h"
#include "spdlogConfig.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class VelocityDensityProfilingTest : public testing::Test {
protected:
    outputWriter::VelocityDensityProfileWriter profileWriter; // Declare the member variable

    void TearDown() override {
        spdlog::drop("VelocityDensityProfileWriter");
    }

    VelocityDensityProfilingTest() : profileWriter("profileTest") {}

    std::vector<std::vector<double>> readCSV(const std::string& filename) {
        std::vector<std::vector<double>> data;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        std::string tmpString;
        while (getline(file, tmpString)) {
            std::vector<double> line;
            std::istringstream numstream(tmpString);
            std::string element;

            while (getline(numstream, element, ',')) {
                line.push_back(std::stod(element));
            }

            data.push_back(line);
        }

        file.close();
        return data;
    }
};


/**
 * @brief checks if the VelocityDensityProfileReader uses the correct number of bins and stores the correct number of particles
 */
TEST_F(VelocityDensityProfilingTest, ProfileReaderUsesTheCorrectNumberOfBinsAndStoresTheCorrectNumberOfParticles) {


ParticleContainerLinkedCell particles = ParticleContainerLinkedCell({10, 10, 10}, 3,
                                    {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}});

    double binSize = 0.5;

    for(int i = 0; i < 20; i++){

        for(int j = 0; j < i; j++){

            Particle p = Particle({binSize * i, 0, 0}, {0, 0, 0}, 1);
            particles.addParticle(p);
        }

    }

    SimulationData simulationData1 = SimulationData();
    simulationData1.setProfileBinNumber(20);




    profileWriter.profileBins(particles,1, simulationData1.getProfileBinNumber());


    std::vector<std::vector<double>> csvData = readCSV("./profileTest_0001.csv");
    for(int i = 0; i < 20; i++){
        EXPECT_EQ(csvData[i][0], i);
    }



}

/**
 * @brief checks if the VelocityDensityProfileReader stores the correct density for every bin
 */
TEST_F(VelocityDensityProfilingTest, ProfileReaderCalculatesTheCorrectDensity) {


    ParticleContainerLinkedCell particles = ParticleContainerLinkedCell({10, 10, 10}, 3,
                                                                        {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}});
    double binSize = 0.5;

    for(int i = 0; i < 20; i++){

        for(int j = 0; j < i; j++){

            Particle p = Particle({binSize * i, 0, 0}, {0, 0, 0}, 1);
            particles.addParticle(p);
        }

    }

    SimulationData simulationData1 = SimulationData();
    simulationData1.setProfileBinNumber(20);




    profileWriter.profileBins(particles,1, simulationData1.getProfileBinNumber());


    std::vector<std::vector<double>> csvData = readCSV("./profileTest_0001.csv");

    double binVolume = binSize * 10 * 10;

    for(int i = 0; i < 20; i++){
       EXPECT_EQ(csvData[i][1], i / binVolume);

    }


}


/**
 * @brief checks if the VelocityDensityProfileReader stores the correct average velocity for every bin
 */
TEST_F(VelocityDensityProfilingTest, ProfileReaderCalculatesTheCorrectAverageVelocity) {


    ParticleContainerLinkedCell particles = ParticleContainerLinkedCell({10, 10, 10}, 3,
                                                                        {{outflow, outflow}, {outflow, outflow}, {outflow, outflow}});
    double binSize = 0.5;

    for(double i = 0; i < 20; i++){

        for(double j = 0; j < i; j++){

            Particle p = Particle({binSize * i, 0, 0}, {j, i, 0}, 1);
            particles.addParticle(p);

        }

    }

    SimulationData simulationData1 = SimulationData();
    simulationData1.setProfileBinNumber(20);




    profileWriter.profileBins(particles,1, simulationData1.getProfileBinNumber());


    std::vector<std::vector<double>> csvData = readCSV("./profileTest_0001.csv");


    for(int i = 0; i < 20; i++){
        double sum = 0.0;
        for(int j = 0; j < i; j++){
            sum += j;
        }
        if(i != 0){
            EXPECT_EQ(csvData[i][2], sum/ i);
        }
        EXPECT_EQ(csvData[i][3], i );
        EXPECT_EQ(csvData[i][4], 0);

    }


}
