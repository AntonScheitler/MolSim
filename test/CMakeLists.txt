# compile binary for computation tests
file(GLOB_RECURSE COMPUTATION_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/computations/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/computations/*.h"
)

add_executable(computationTests ${COMPUTATION_TESTS})
target_link_libraries(computationTests
    src
    gtest_main
)

## compile binary for io tests
file(GLOB_RECURSE INPUT_OUTPUT_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/io/inputReader/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/io/inputReader/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/io/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/io/outputWriter/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/io/outputWriter/*.h"

)

add_executable(inputOutput ${INPUT_OUTPUT_TESTS})
target_link_libraries(inputOutput
    src
    gtest_main
)

# compile binary for particle tests
file(GLOB_RECURSE PARTICLE_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/particle/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/particle/*.h"
)

add_executable(particleTests ${PARTICLE_TESTS})
target_link_libraries(particleTests
    src
    gtest_main
)

# add tests to ctest
enable_testing()
include(GoogleTest)
gtest_discover_tests(computationTests)
gtest_discover_tests(inputOutput)
gtest_discover_tests(particleTests)
