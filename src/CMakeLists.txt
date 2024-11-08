# add all files in src, except for MolSim.cpp
file(GLOB_RECURSE SRCFILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM SRCFILES "${CMAKE_CURRENT_SOURCE_DIR}/MolSim.cpp")

# create library for all src files
add_library(src ${SRCFILES})
target_include_directories(src
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
    ${PSEMolDyn_GroupF_SOURCE_DIR}/libs/libxsd
)
target_link_libraries(src
    xerces-c
)

# create molsim executable
add_executable(molsim MolSim.cpp)
# link the executable to the src library
target_link_libraries(molsim
        src
        spdlog::spdlog
)

# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(molsim
    PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    # disable some spam warnings for icpc...
    $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)
