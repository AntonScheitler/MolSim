#include "FileReader.h"
#include "JsonFileReader.h"
#include "TxtFileReader.h"
#include "spdlogConfig.h"

namespace inputReader {
        FileReader::FileReader(SimulationData& simDataArg) : simData(simDataArg) {
            this->logger = spdlog::stdout_color_st("FileReader");
            SPDLOG_LOGGER_DEBUG(logger, "Initialized FileReader");
        }

        FileReader::~FileReader() {
            spdlog::drop("FileReader");
        }

        void FileReader::readFile(ParticleContainer &particles, char *filename) {
            // Extract the file extension
            std::string filestr(filename);
            std::string fileFormat = filestr.substr(filestr.find_last_of('.') + 1);

            // Decide which reader to instantiate based on the file extension
            if (fileFormat == "json") {
                readJson(particles, filename);
            } else if (fileFormat == "txt") {
                readTxt(particles, filename);
            } else {
                SPDLOG_LOGGER_ERROR(logger, "Error: unsupported file format", filename);
                exit(-1);
            }
        }

        void FileReader::readJson(ParticleContainer &particles, char *filename) {
            JsonFileReader jsonReader;
            if (simData.getSimType() == comet) {
                jsonReader.readCometFile(particles, filename);
            } else {
                jsonReader.readCollisionFile(particles, filename);
            }
        }

        void FileReader::readTxt(ParticleContainer &particles, char *filename) {
            TxtFileReader txtReader;
            if (simData.getSimType() == comet) {
                txtReader.readCometFile(particles, filename);
            } else {
                txtReader.readCollisionFile(particles, filename);
            }
        }


} // namespace inputReader
