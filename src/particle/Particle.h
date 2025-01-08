/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

/**
 * @brief Class for modeling particles, where each Particle has three-dimensional position, velocity and force that is
 * effective on it. Mass of a Particle can also be specified. Use the class ParticleContainer for storing multiple
 * particles and iterating over them.
 */
class Particle {

private:
    std::array<double, 3> oldX;
    /**
     * @brief Position of the particle
     */
    std::array<double, 3> x;

    /**
     * @brief Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * @brief Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * @brief Force which was effective on this particle
     */
    std::array<double, 3> oldF;

    /**
     * @brief Mass of this particle
     */
    double m;

    /**
     * @brief the id of a particle. used for comparison
     */
    size_t id;

    /**
     * @brief a static counter which is used to give each particle a unique id
     */
    static size_t nextId;

    /**
     * @brief Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;


    /**
     * @brief the epsilon value of this particle
     */
    double epsilon;

    /**
     * @brief the sigma value of this particle
     */
    double sigma;


    /**
     * @brief specifies whether or not this particle is part of the simulation
     */
    bool active;

    /**
     * @brief specifies whether this particle should not change its position
     */
    bool fixed;

public:
    explicit Particle(int typeArg = 0);

    Particle(const Particle &other);

    Particle(
            // for visualization, we need always 3 coordinates
            // -> in case of 2d, we use only the first and the second
            std::array<double, 3> xArg, std::array<double, 3> vArg, double mArg,
            int typeArg = 0);

    /**
     * @brief destruct the Particle
     */
    virtual ~Particle();

    /**
     * @brief gets the old position of this particle
     * @return old position
     */
    const std::array<double, 3> &getOldX() const;

    /**
     * @brief set the old position of this particle
     * @param oldXArg old position
     */
    void setOldX(std::array<double, 3> oldXArg);

    /**
    * @brief Returns the position of the particle
    * @return position of the particle
    */
    const std::array<double, 3> &getX() const;

    /**
     * @brief Set the position of the particle
     * @param newX new position of the particle
     */
    void setX(std::array<double, 3> newX);

    /**
     * @brief Get the velocity of the particle
     * @return velocity of particle
     */
    const std::array<double, 3> &getV() const;

    /**
     * @brief Set the new velocity of the particle
     * @param newV new velocity
     */
    void setV(std::array<double, 3> newV);

    /**
     * @brief Get the force acting on this particle
     * @return force of particle
     */
    const std::array<double, 3> &getF() const;

    /**
     * @brief Get the old force that was acting on this particle
     * @return old force of particle
     */
    const std::array<double, 3> &getOldF() const;

    /**
     * @brief Set the force of this particle
     * @param newF new force
     */
    void setF(std::array<double, 3> newF);

    /**
     * @brief Set the old force of this particle
     * @param newF old force
     */
    void setOldF(std::array<double, 3> newF);

    /**
     * @brief Set the new mass of this particle
     * @param newM new mass
     */
    void setM(double newM);

    /**
     * @brief Get the mass of this particle
     * @return mass of this particle
     */
    double getM() const;

    /**
     * @brief Get the type of this particle
     * @return type of this particle
     */
    int getType() const;

    /**
     * @brief the the id of the particle
     */
    size_t getId() const;

    /**
     * @brief checks if two particles are equal
     * @param other the particle to compare with
     */
    bool operator==(const Particle &other) const;

    /**
     * @brief converts information about this particle into a string
     */
    std::string toString() const;


    /**
     * @brief returns the sigma value of this particle
     * @return the sigma value
     */
    double getSigma() const;

    /**
     * @brief sets the sigma value of this particle
     * @param sigma the sigma value
     */
    void setSigma(double sigma);

    /**
     * @brief returns the epsilon value of this particle
     * @return the epsilon value
     */
    double getEpsilon() const;

    /**
     * @brief sets the epsilon value of this particle
     * @param epsilon the epsilon value
     */
    void setEpsilon(double epsilon);

    /**
     * @brief get the active status of this particle
     * @return the active status of this particle
     */
    const bool getActive() const;

    /**
     * @brief set the active status of this particle
     */
    void setActive(bool newActive);

    /**
     * @brief sets the type of this particle (to differentiate between different objects consisting of particles)
     * @param typeArg the type of this particle
     */
    void setType(int typeArg);
    /**
     * @brief returns true if this particle is fixed and therefore does not change its position
     */
    bool isFixed();
    /**
     * @brief specify whether this particle should be fixed or not
     * @param fixedArg true : fixed, false : not fixed
     */
    void setFixed(bool fixedArg);
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
