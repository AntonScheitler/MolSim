#include <computations/profile/VelocityDensityProfile.h>
#include <functional>
#include <utils/ArrayUtils.h>

std::vector<VelocityDensityProfile::binInfo>  VelocityDensityProfile::determineProfile(ParticleContainerLinkedCell& particles, size_t numBins) {
    double binLength = particles.getDomainSize()[0] / numBins;
    double binSize = binLength * particles.getDomainSize()[1] * particles.getDomainSize()[2];
    std::vector<binInfo> binInfos = {};

    for (size_t i = 0; i < numBins; i++) {
        binInfo info{};
        info.sumVelocities = {0, 0, 0};
        info.numParticles = 0;


        for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
            Particle& particle = *it;

            if (particle.getX()[0] >= binLength * i && particle.getX()[0] < binLength * (i + 1)) {
                info.numParticles++;
                info.sumVelocities = ArrayUtils::elementWisePairOp(info.sumVelocities, particle.getV(), std::plus<>());
            }
        }
        binInfos.push_back(info);
    }


    for (binInfo& binInfo : binInfos) {
        binInfo.avgDensity = binInfo.numParticles / binSize;
        if(binInfo.numParticles == 0){
            binInfo.avgVelocity = {0, 0, 0};
        }else {
            binInfo.avgVelocity = ArrayUtils::elementWiseScalarOp(1.0 / binInfo.numParticles, binInfo.sumVelocities,
                                                                  std::multiplies<>());
            std::array<double, 3> squared =ArrayUtils::elementWisePairOp(binInfo.avgVelocity, binInfo.avgVelocity, std::multiplies<>());
            binInfo.avgVelocityNorm = sqrt(squared[0] + squared[1] + squared[2]);
        }
    }

<<<<<<< Updated upstream
=======

    for (struct binInfo& binInfo : binInfos) {
        binInfo.avgDensity = binInfo.numParticles / binSize;
        binInfo.avgVelocity = ArrayUtils::elementWiseScalarOp(1.0 / binInfo.numParticles, binInfo.sumVelocities, std::multiplies<>());

    }

>>>>>>> Stashed changes
    return binInfos;
}
