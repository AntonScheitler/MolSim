#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() {
    this->particles = std::vector<Particle>{};
}
ParticleContainer::ParticleContainer(const std::vector<Particle>& particles) {
    this->particles = particles;
}

void ParticleContainer::addParticle(const Particle &particle) {
    this->particles.push_back(particle);
}

void ParticleContainer::removeParticle(Particle &particle) {
    //TODO
}

Iterator ParticleContainer::begin() {
    if(particles.size() == 0) return nullptr;
    return Iterator(&this->particles.at(0));
}
Iterator ParticleContainer::end() {
    if(particles.size() == 0) return nullptr;
    return Iterator(&this->particles.at(this->particles.size() - 1));
}

