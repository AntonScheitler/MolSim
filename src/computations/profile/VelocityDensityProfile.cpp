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
        binInfos.push_back(info);

        for (auto it = particles.beginNonFixedParticles(); it != particles.endNonFixedParticles(); ++it) {
            Particle& particle = *it;

            if (particle.getX()[0] >= binLength * i && particle.getX()[0] < binLength * (i + 1)) {
                binInfos[0].numParticles++;
                binInfos[0].sumVelocities = ArrayUtils::elementWisePairOp(binInfos[0].sumVelocities, particle.getV(), std::plus<>());
            }
        }
    }


    for (struct binInfo& binInfo : binInfos) {
        binInfo.avgDensity = binInfo.numParticles / binSize;
        binInfo.avgVelocity = ArrayUtils::elementWiseScalarOp(1.0 / binInfo.numParticles, binInfo.sumVelocities, std::multiplies<>());
    }

    return binInfos;
}
