#include "../../computations/profile/VelocityDensityProfile.h"
#include "FileWriter.h"

namespace outputWriter {

    class VelocityDensityProfileWriter  {
    public:

        VelocityDensityProfileWriter(std::string baseName);

        /**
         * @brief plots bin profiles and writes them to a csv file
         * @param particles the particles to plot
         * @param iteration number of the current iteration
         * @param bins number of bins to separate the domain into
         */
        void profileBins(ParticleContainerLinkedCell& particles, int iteration, int bins);

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

