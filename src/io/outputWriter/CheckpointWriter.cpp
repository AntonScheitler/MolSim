
#include "CheckpointWriter.h"
#include "particle/container/ParticleContainer.h"
#include "spdlogConfig.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>


namespace outputWriter {

    CheckpointWriter::CheckpointWriter(std::string baseName) : baseName(baseName) {
        this->logger = spdlog::stdout_color_st("CheckpointWriter");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized CheckpointWriter");
    }


    void CheckpointWriter::plotParticles(ParticleContainer &particles, int iteration) {
        std::stringstream strstr;
        strstr << baseName << ".txt";

        std::ofstream file(strstr.str().c_str());

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        file << particles.size() << "\n";

        for (Particle& particle : particles) {
            file << plotParticle(particle) << "\n";
        }

        file.close();
    }

    std::string CheckpointWriter::plotParticle(Particle &p) {
        std::ostringstream particleParams;

        particleParams << " " << p.getOldX()[0] << " " << p.getOldX()[1] << " " << p.getOldX()[2] << "    ";

        particleParams << " " << p.getX()[0] << " " << p.getX()[1] << " " << p.getX()[2] << "    ";

        particleParams << " " << p.getV()[0] << " " << p.getV()[1] << " " << p.getV()[2] << "    ";

        particleParams << " " << p.getF()[0] << " " << p.getF()[1] << " " << p.getF()[2] << "    ";

        particleParams << " " << p.getOldF()[0] << " " << p.getOldF()[1] << " " << p.getOldF()[2] << "    ";

        particleParams << p.getM() << "    ";

        particleParams << p.getType() << "    ";

        particleParams << p.getEpsilon() << "    ";

        particleParams << p.getSigma();

        return particleParams.str();
    }

} // namespace outputWriter
