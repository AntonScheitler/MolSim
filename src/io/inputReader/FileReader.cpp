#include "FileReader.h"
#include "JsonFileReader.h"
#include "TxtFileReader.h"
#include "io/inputReader/xmlReader/XMLFileReader.h"
#include "spdlogConfig.h"


namespace inputReader {

    FileReader::FileReader(SimulationData &simDataArg) : simData(simDataArg) {
        this->logger = spdlog::stdout_color_st("FileReader");
        SPDLOG_LOGGER_DEBUG(logger, "Initialized FileReader");
    }

    FileReader::~FileReader() {
        spdlog::drop("FileReader");
    }

    void FileReader::readFile(char *filename) {
        // Extract the file extension
        std::string filestr(filename);
        std::string fileFormat = filestr.substr(filestr.find_last_of('.') + 1);

        // Decide which reader to instantiate based on the file extension
        if (fileFormat == "json") {
            readJson(simData, filename);
        } else if (fileFormat == "txt") {
            readTxt(simData, filename);
        } else if (fileFormat == "xml") {
            readXML(simData, filename);
        } else {
            SPDLOG_LOGGER_ERROR(logger, "Error: unsupported file format", filename);
            exit(-1);
        }
    }

    void FileReader::readJson(SimulationData &simData, char *filename) {
        JsonFileReader jsonReader{simData};
        jsonReader.readFile(simData, filename);
    }

    void FileReader::readTxt(SimulationData &simData, char *filename) {
        TxtFileReader txtReader{simData};
        if (simData.getSimType() == comet) {
            txtReader.readCometFile(simData, filename);
        } else {
            txtReader.readCollisionFile(simData, filename);
        }
    }

    void FileReader::readXML(SimulationData &simData, char *filename) {
        XMLFileReader xmlFileReader(simData);
        xmlFileReader.readFile(filename);
    }

} // namespace inputReader
