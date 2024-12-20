#pragma once
//#include <particle/container/ParticleContainer.h>

#include <array>
namespace inputReader {
    /**
     * @brief class for generating specific structures consisting of particles
     */
    class ParticleGenerator {
    public:
        /**
         * @brief generates a cuboid consisting of particles
         * @param particles the container to write the particles into
         * @param x the coordinates of the left bottom corner
         * @param v the initial velocity of the cuboid
         * @param d the size of the cuboid
         * @param m the mass of one particle
         * @param h the space between each particle
         * @param type the type of particles
         * @param e epsilon value of particles
         * @param s sigma value of particles
         */
        static void generateCuboid(ParticleContainer& particles, std::array<double, 3> x, std::array<double, 3> v,
                                      std::array<int, 3> d, double m, double h, int type, double e, double s);
        /**
         * @brief generates a disc consisting of particles
         * @param particle the container to write the particles into
         * @param center the coordinates of the center of the disc
         * @param v the initial velocity of the whole disc (i.e. of each particle)
         * @param r the radius of the disc
         * @param m the mass of each particle
         * @param h the space between particles
         * @param type the type of particles
         * @param e epsilon value of particles
         * @param s sigma value of particles
         */
        static void generateDisc(ParticleContainer& particles, std::array<double, 3> center,
                                            std::array<double, 3> v, double r, double m, double h, int type, double e, double s);
    };
}
