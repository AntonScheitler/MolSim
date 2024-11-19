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
    /**
     * @brief returns a ParticleIterator which points to the beginning of the ParticleContainer. This makes iteration over
     * separate particles the default when using a for-each loop
     * @return a ParticleIterator which points to the beginning of the ParticleContainer
     */
    virtual ParticleIterator begin() = 0;
    /**
     * @brief returns a ParticleIterator which points to the end of the ParticleContainer. This makes iteration over
     * separate particles the default when using a for-each loop
     * @return a ParticleIterator which points to the end of the ParticleContainer
     */
    virtual ParticleIterator end() = 0;

    /**
     * @brief returns a PairParticleIterator which points to the first pair of the ParticleContainer
     * @return a PairParticleIterator which points to the first pair of the ParticleContainer
     */
    virtual PairParticleIterator beginPairParticle() = 0;

    /**
     * @brief returns a PairParticleIterator which points to the theoretical pair after the last pair of the ParticleContainer
     * @return a PairParticleIterator which points to the theoretical pair after the last pair of the ParticleContainer
     */
    virtual PairParticleIterator endPairParticle() = 0;
};