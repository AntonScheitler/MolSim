/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"
#include "spdlogConfig.h"

#include "utils/ArrayUtils.h"
#include <array>
#include <iostream>
#include <vector>
#include <memory>


size_t Particle::nextId = 0;

Particle::Particle(int typeArg) {
    active = true;
    id = nextId++;
    type = typeArg;
    f = {0., 0., 0.};
    oldF = {0., 0., 0.};
    epsilon = 0;
    sigma = 0;

    fixed = false;

    directNeighborsIndices = std::vector<int>();
    diagonalNeighborsIndices = std::vector<int>();

    SPDLOG_DEBUG("Particle generated!"); // using default logger, do not create logger for each particle
}

Particle::Particle(const Particle &other) {
    active = other.active;
    id = other.id;
    nextId = other.nextId;
    oldX = other.oldX;
    x = other.x;
    v = other.v;
    f = other.f;
    oldF = other.oldF;
    m = other.m;
    type = other.type;
    epsilon = other.epsilon;
    sigma = other.sigma;

    fixed = other.fixed;

    directNeighborsIndices = other.directNeighborsIndices;
    diagonalNeighborsIndices = other.diagonalNeighborsIndices;

    SPDLOG_DEBUG("Particle generated by copy!");
}

// Todo: maybe use initializer list instead of copy?
Particle::Particle(std::array<double, 3> xArg, std::array<double, 3> vArg,
                   double mArg, int typeArg) {
    active = true;
    id = nextId++;
    oldX = {0., 0., 0.};
    x = xArg;
    v = vArg;
    m = mArg;
    type = typeArg;
    f = {0., 0., 0.};
    oldF = {0., 0., 0.};
    epsilon = 0;
    sigma = 0;
    directNeighborsIndices = std::vector<int>();
    diagonalNeighborsIndices = std::vector<int>();
    SPDLOG_DEBUG("Particle generated!");
}

Particle::Particle(std::array<double, 3> xArg, std::array<double, 3> vArg,
                   double mArg, int typeArg, double epsilonArg, double sigmaArg, bool fixedArg) {
    active = true;
    id = nextId++;
    oldX = {0., 0., 0.};
    x = xArg;
    v = vArg;
    m = mArg;
    type = typeArg;
    f = {0., 0., 0.};
    oldF = {0., 0., 0.};
    epsilon = epsilonArg;
    sigma = sigmaArg;
    fixed = fixedArg;
    SPDLOG_DEBUG("Particle generated!");
}

Particle::~Particle() {
    SPDLOG_DEBUG("Particle destructed!");
}

const std::array<double, 3> &Particle::getX() const { return x; }

void Particle::setX(std::array<double, 3> newX) { x = newX; }

const std::array<double, 3> &Particle::getV() const { return v; }

void Particle::setV(std::array<double, 3> newV) { v = newV; }

const std::array<double, 3> &Particle::getF() const { return f; }

const std::array<double, 3> &Particle::getOldF() const { return oldF; }

void Particle::setF(std::array<double, 3> newF) { f = newF; }

void Particle::setOldF(std::array<double, 3> newF) { oldF = newF; }

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

size_t Particle::getId() const { return id; }

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f << " oldF: " << oldF << " type: " << type;
    return stream.str();
}

bool Particle::operator==(const Particle &other) const {
    return id == other.id;
}

void Particle::setM(double newM) {
    this->m = newM;
}

const std::array<double, 3> &Particle::getOldX() const {
    return this->oldX;
}

void Particle::setOldX(std::array<double, 3> oldXArg) {
    this->oldX = oldXArg;
}


double Particle::getEpsilon() const {
    return epsilon;
}

double Particle::getSigma() const {
    return sigma;
}

void Particle::setSigma(double sigmaArg) {
    this->sigma = sigmaArg;
}

void Particle::setType(int typeArg) {
    Particle::type = typeArg;
}

void Particle::setEpsilon(double epsilonArg) {
    this->epsilon = epsilonArg;
}

const bool Particle::getActive() const {
    return active;
};

void Particle::setActive(bool newActive) {
    active = newActive;
}

bool Particle::isFixed() {
    return fixed;
}

void Particle::setFixed(bool fixedArg) {
    fixed = fixedArg;
};

int Particle::getNeighborIdx(DirectNeighborPos pos) {
    switch (pos) {
        case left:
            return directNeighborsIndices[1];
        case right:
            return directNeighborsIndices[2];
        case top:
            return directNeighborsIndices[3];
        case bottom:
            return directNeighborsIndices[0];
    }
    return -1;
}

int Particle::getDiagNeighborIdx(DiagonalNeighborPos pos) {
    switch (pos) {
        case topLeft:
            return diagonalNeighborsIndices[2];
        case topRight:
            return diagonalNeighborsIndices[3];
        case bottomLeft:
            return diagonalNeighborsIndices[0];
        case bottomRight:
            return diagonalNeighborsIndices[1];
    }
    return -1;
}

void Particle::addNeighborIdx(int particleIDArg) {
    directNeighborsIndices.push_back(particleIDArg);
}

void Particle::addDiagNeighborIdx(int particleIDArg) {
    diagonalNeighborsIndices.push_back(particleIDArg);
}

std::vector<int>& Particle::getAllDirectNeighborIndices() {
    return directNeighborsIndices; 
}

std::vector<int>& Particle::getAllDiagonalNeighborIndices() {
    return diagonalNeighborsIndices; 
}

std::ostream &operator<<(std::ostream &stream, Particle &p) {
    stream << p.toString();
    return stream;
}


