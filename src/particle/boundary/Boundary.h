#pragma once
/**
 * @brief the types of boundaries this linked cell container can have
 */
#include <array>
enum BoundaryTypes {
    outflow,
    ghost
};

/**
 * @brief a struct containing information on the configuration of the boundaries for a linked cell container
 * the first index of a member marks the boundary type at the beginning of this axis and the second member marks that of
 * its end. i.e. x[0] is the boundary type to the "left" of the mesh and x[1] is that to the "right" of the mesh
 */
struct boundaryConfig {
            std::array<BoundaryTypes, 2> x;
            std::array<BoundaryTypes, 2> y;
            std::array<BoundaryTypes, 2> z;
};
