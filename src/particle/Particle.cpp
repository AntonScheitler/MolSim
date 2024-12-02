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

Particle::Particle(int typeArg) {
    type = typeArg;
    f = {0., 0., 0.};
    oldF = {0., 0., 0.};
    SPDLOG_DEBUG("Particle generated!"); // using default logger, do not create logger for each particle
}

Particle::Particle(const Particle &other) {
    oldX = other.oldX;
    x = other.x;
    v = other.v;
    f = other.f;
    oldF = other.oldF;
    m = other.m;
    type = other.type;
    SPDLOG_DEBUG("Particle generated by copy!");
}

// Todo: maybe use initializer list instead of copy?
Particle::Particle(std::array<double, 3> xArg, std::array<double, 3> vArg,
                   double mArg, int typeArg) {
    oldX = {0., 0., 0.};
    x = xArg;
    v = vArg;
    m = mArg;
    type = typeArg;
    f = {0., 0., 0.};
    oldF = {0., 0., 0.};
    SPDLOG_DEBUG("Particle generated!");
}

Particle::~Particle() { SPDLOG_DEBUG("Particle destructed!"); }

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

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f << " oldF: " << oldF << " type: " << type;
    return stream.str();
}

bool Particle::operator==(const Particle &other) const {
    return (x == other.x) and (v == other.v) and (f == other.f) and
           (type == other.type) and (m == other.m) and (oldF == other.oldF);
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

std::ostream &operator<<(std::ostream &stream, Particle &p) {
    stream << p.toString();
    return stream;
}
