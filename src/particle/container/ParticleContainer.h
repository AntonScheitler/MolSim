#include "../Particle.h"
#include "../iterator/pairParticleIterator/PairParticleIterator.h"

class ParticleContainer {
private:

public:
    ParticleContainer() = default;
    virtual ~ParticleContainer() = default;

    /**
     * @brief adds a particle to the container
     * @param particle the particle to be added
     */
    virtual void addParticle(const Particle &particle) = 0;

    /**
     * @brief returns the number of particles in the ParticleContainer
     * @return the number of particles in the ParticleContainer
     */
    virtual int size() = 0;
    /**
     * @brief returns the average velocity that exists due to brownian motion
     * @return the average velocity that exists due to brownian motion
     */
    virtual double getAverageVelocity() = 0;
    /**
     * @brief sets the average velocity that exists due to brownian motion
     * @param averageVelocityArg the velocity to set the average velocity to
     */
    virtual void setAverageVelocity(double averageVelocityArg) = 0;
    virtual Particle& getParticle(int index) = 0;
};
