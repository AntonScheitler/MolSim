#include <io/inputReader/XMLReader/XMLFileReader.h>
#include <gtest/gtest.h>
#include <particle/container/ParticleContainerLinkedCell.h>
#include <particle/boundary/Boundary.h>

class XMLFileReaderTest : public testing::Test {

};

/**
 * @brief tests the xmlFileReader by reading in an xml file and checking the values of SimulationData and the
 * particles
 */
TEST_F(XMLFileReaderTest, XMlFileReaderCorrectInputTest) {

    SimulationData simData{};
    inputReader::XMLFileReader fileReader{simData};

    // read in dummy file

    fileReader.readFile((char*) "../../testInput/test-input.xml");

    // check if all correct
    EXPECT_DOUBLE_EQ(simData.getDeltaT(), 0.0005);
    EXPECT_DOUBLE_EQ(simData.getEndTime(), 50);
    auto containerLinkedCell = dynamic_cast<ParticleContainerLinkedCell *>(&(simData.getParticles()));
    if (containerLinkedCell) {
        std::array<double, 3> ds = containerLinkedCell->getDomainSize();
        EXPECT_DOUBLE_EQ(ds[0], 40);
        EXPECT_DOUBLE_EQ(ds[1], 40);
        EXPECT_DOUBLE_EQ(ds[2], 40);

        EXPECT_DOUBLE_EQ(containerLinkedCell->getCutoffRadius(), 3);

        struct boundaryConfig boundaryConfigExpected = {{periodic, periodic},
                                                        {outflow,  reflect},
                                                        {outflow,  outflow}};


        struct boundaryConfig boundaryConfigActual = containerLinkedCell->getBoundaryConfig();
        EXPECT_EQ(boundaryConfigExpected.x, boundaryConfigActual.x);
        EXPECT_EQ(boundaryConfigExpected.y, boundaryConfigActual.y);
        EXPECT_EQ(boundaryConfigExpected.z, boundaryConfigActual.z);

        EXPECT_DOUBLE_EQ(simData.getGrav()[1], -15);

        // first cuboid
        // take first particle and check its attributes
        int index = containerLinkedCell->continuousCoordsToIndex({1, 2, 3});
        Cell& cell = containerLinkedCell->getMesh()[index];
        Particle &p = containerLinkedCell->getParticleAt(cell.getParticlesIndices()[0]);

        EXPECT_DOUBLE_EQ(p.getM(), 1.0);
        EXPECT_DOUBLE_EQ(p.getEpsilon(), 1.0);
        EXPECT_DOUBLE_EQ(p.getSigma(), 1.2);

        // velocity
        EXPECT_DOUBLE_EQ(p.getV()[0], 3);
        EXPECT_DOUBLE_EQ(p.getV()[1], 2);
        EXPECT_DOUBLE_EQ(p.getV()[2], 1);

        // second cuboid
        index = containerLinkedCell->continuousCoordsToIndex({20, 21, 22});
        cell = containerLinkedCell->getMesh()[index];
        p = containerLinkedCell->getParticleAt(cell.getParticlesIndices()[0]);

        EXPECT_DOUBLE_EQ(p.getM(), 2.0);
        EXPECT_DOUBLE_EQ(p.getEpsilon(), 2.0);
        EXPECT_DOUBLE_EQ(p.getSigma(), 1.1);

        // velocity
        EXPECT_DOUBLE_EQ(p.getV()[0], 7.7);
        EXPECT_DOUBLE_EQ(p.getV()[1], 6.6);
        EXPECT_DOUBLE_EQ(p.getV()[2], 5.5);

        // thermo
        EXPECT_DOUBLE_EQ(simData.getInitialTemp(), 40);
        EXPECT_DOUBLE_EQ(simData.getThermoFrequency(), 1000);
    } else EXPECT_TRUE(false);
}
