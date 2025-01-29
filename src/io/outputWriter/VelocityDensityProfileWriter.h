#include "../../computations/profile/VelocityDensityProfile.h"
#include "FileWriter.h"

namespace outputWriter {

<<<<<<< Updated upstream
    class VelocityDensityProfileWriter  {
=======
    class VelocityDensityProfileWriter : public FileWriter {
>>>>>>> Stashed changes
    public:

        VelocityDensityProfileWriter(std::string baseName);

        /**
         * @brief plots bin profiles and writes them to a csv file
         */
<<<<<<< Updated upstream
        void profileBins(ParticleContainerLinkedCell& particles, int iteration, int bins);
=======
        void profileBins( std::vector<VelocityDensityProfile::binInfo> &binInfos, int iteration);
>>>>>>> Stashed changes

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

