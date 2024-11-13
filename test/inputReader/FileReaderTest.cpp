#include "../../src/inputReader/FileReader.h"
#include <gtest/gtest.h>

class FileReaderTest : public testing::Test {

protected:
    SimulationData simulationData{};

    void testCometSim(char *inputFile);
    void testCollisionSim(char *inputFile);

};

/**
 * @brief asserts whether the values of the given 3d-array match the given values
 * @param v 3d array to check against
 * @param x0 value 0
 * @param x1 value 1
 * @param x2 value 2
 */
void assertArray(std::array<double, 3> v, double x0, double x1, double x2) {
    EXPECT_DOUBLE_EQ(v[0], x0);
    EXPECT_DOUBLE_EQ(v[1], x1);
    EXPECT_DOUBLE_EQ(v[2], x2);
}

/**
 * @brief checks if all particles have the same position, velocity and mass as specified in the test input file
 * @param p particleContainer containing the particles
 */
void assertParticlesCometSim(ParticleContainer &p) {
    // particle 0
    assertArray(p.getParticle(0).getX(), 0, 0, 0);
    assertArray(p.getParticle(0).getV(), 0, 0, 0);
    EXPECT_DOUBLE_EQ(p.getParticle(0).getM(), 1.0);

    // particle 1
    assertArray(p.getParticle(1).getX(), 0, 1, 0);
    assertArray(p.getParticle(1).getV(), -1, 0, 0);
    EXPECT_DOUBLE_EQ(p.getParticle(1).getM(), 3.0e-6);

    // particle 2
    assertArray(p.getParticle(2).getX(), 0.0, 5.36, 0.0);
    assertArray(p.getParticle(2).getV(), -0.425, 0.0, 0.0);
    EXPECT_DOUBLE_EQ(p.getParticle(2).getM(), 9.55e-4);

    // particle 3
    assertArray(p.getParticle(3).getX(), 34.75, 0.0, 0.0);
    assertArray(p.getParticle(3).getV(), 0.0, 0.0296, 0.0);
    EXPECT_DOUBLE_EQ(p.getParticle(3).getM(), 1.0e-14);
}

/**
 * @brief checks if all particles have the same position, velocity and mass as specified in the test input file
 * @param p particleContainer containing the particles
 */
void assertParticlesCollisionSim(ParticleContainer &p) {
    // cuboid 0
    // particle 0
    assertArray(p.getParticle(0).getX(), 0, 0, 0);
    assertArray(p.getParticle(0).getV(), 0, 0, 7.4);
    EXPECT_DOUBLE_EQ(p.getParticle(0).getM(), 1.0);

    // particle 1
    assertArray(p.getParticle(1).getX(), 1.1225, 0, 0);
    assertArray(p.getParticle(1).getV(), 0, 0, 7.4);
    EXPECT_DOUBLE_EQ(p.getParticle(1).getM(), 1);

    // cuboid 1
    // particle 0
    assertArray(p.getParticle(2).getX(), 15, 15, 0);
    assertArray(p.getParticle(2).getV(), 0, -10, 0);
    EXPECT_DOUBLE_EQ(p.getParticle(2).getM(), 3);

    // particle 1
    assertArray(p.getParticle(3).getX(), 15, 16.1225, 0);
    assertArray(p.getParticle(3).getV(), 0.0, -10, 0);
    EXPECT_DOUBLE_EQ(p.getParticle(3).getM(), 3);
}

/**
 * @brief reads the specified file and checks whether the particles returned by the fileReader match the values
 * specified in the test input file for comet simulation
 * @param inputFile path of the input file
 */
void FileReaderTest::testCometSim(char *inputFile) {
    simulationData.setSimType(simulationType::comet);
    inputReader::FileReader fileReader = inputReader::FileReader(simulationData);
    ParticleContainer p{};
    fileReader.readFile(p, inputFile);
    assertParticlesCometSim(p);
}

/**
 * @brief reads the specified file and checks whether the particles returned by the fileReader match the values
 * specified in the test input file for collision simulation
 * @param inputFile path of the input file
 */
void FileReaderTest::testCollisionSim(char *inputFile) {
    simulationData.setSimType(simulationType::collision);
    inputReader::FileReader fileReader = inputReader::FileReader(simulationData);
    ParticleContainer p{};
    fileReader.readFile(p, inputFile);
    assertParticlesCollisionSim(p);
}

/**
 * @brief tests the jsonFileReader for comet simulation input file
 */
TEST_F(FileReaderTest, JsonFileReaderCometSimInputTest) {
    testCometSim((char *) "../../testInput/test-input-comet.json");
}

/**
 * @brief tests the txtFileReader for comet simulation input file
 */
TEST_F(FileReaderTest, TxtFileReaderCometSimInputTest) {
    testCometSim((char *) "../../testInput/test-input-comet.txt");
}

/**
 * @brief tests the jsonFileReader for comet simulation input file
 */
TEST_F(FileReaderTest, JsonFileReaderCollisionSimInputTest) {
    testCollisionSim((char *) "../../testInput/test-input-cuboids.json");
}

/**
 * @brief tests the txtFileReader for comet simulation input file
 */
TEST_F(FileReaderTest, TxtFileReaderCollisionSimInputTest) {
    testCollisionSim((char*) "../../testInput/test-input-cuboids.txt");
}


