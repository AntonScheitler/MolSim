#include "VelocityDensityProfileWriter.h"
#include "spdlogConfig.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>


namespace outputWriter {

    VelocityDensityProfileWriter::VelocityDensityProfileWriter(std::string baseName) : baseName(baseName) {
        this->logger = spdlog::stdout_color_st("VelocityDensityProfileWriter");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized VelocityDensityProfileWriter");
    }


    void VelocityDensityProfileWriter::profileBins(ParticleContainerLinkedCell& particles, int iteration, int bins) {
        std::stringstream strstr;
        strstr << baseName << "_" << std::setfill('0') << std::setw(4) << iteration
               << ".csv";

        std::ofstream file(strstr.str().c_str());

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        std::vector<VelocityDensityProfile::binInfo> binInfos = VelocityDensityProfile::determineProfile(particles, bins);

        for (const VelocityDensityProfile::binInfo& binInfo: binInfos) {
            file << plotBin(binInfo) << "\n";
        }
        file.close();
    }

    std::string VelocityDensityProfileWriter::plotBin(const VelocityDensityProfile::binInfo &binInfo) {
        std::ostringstream binParams;

        binParams << binInfo.numParticles << ", " << binInfo.avgDensity << ", "  << binInfo.avgVelocity[0] << ", "  << binInfo.avgVelocity[1] << ", "  << binInfo.avgVelocity[2];

        return binParams.str();
    }

} // namespace outputWriter
