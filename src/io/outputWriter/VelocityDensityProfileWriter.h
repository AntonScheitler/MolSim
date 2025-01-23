#include "../../computations/profile/VelocityDensityProfile.h"
#include "FileWriter.h"

namespace outputWriter {

    class VelocityDensityProfileWriter : public FileWriter {
    public:

        VelocityDensityProfileWriter(std::string baseName);

        /**
         * @brief plots bin profiles and writes them to a csv file
         */
        void profileBins( std::vector<VelocityDensityProfile::binInfo> &binInfos, int iteration);

    private:

        std::string baseName = "profile";
        std::shared_ptr<spdlog::logger> logger;


        /**
         * @brief plot number of particles, density and average velocity of bin.
         *
         */
        std::string plotBin(const VelocityDensityProfile::binInfo &binInfo);



    };

} // namespace outputWriter

