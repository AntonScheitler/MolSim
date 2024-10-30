#include "particle/ParticleContainer.h"

namespace forces {
    /**
     * @brief computes the gravitational forces between the planets and changes them in-place
     * @param planets the planets, for which the gravitational forces need to be computed
     */
    void computeGravity(ParticleContainer &planets);
}
