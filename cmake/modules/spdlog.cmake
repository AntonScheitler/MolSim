include(ExternalProject)

ExternalProject_Add(spdlog
        PREFIX spdlog
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs/spdlog
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${STAGING_DIR}
        -DSPDLOG_BUILD_SHARED=OFF
)

add_dependencies(MolSim spdlog)