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

## compile binary for inputReader tests
file(GLOB_RECURSE INPUT_READER_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/inputReader/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/inputReader/*.h"
)

add_executable(inputReaderTests ${INPUT_READER_TESTS})
target_link_libraries(inputReaderTests
    src
    gtest_main
)

# compile binary for particle tests
file(GLOB_RECURSE PARTICLE_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/particle/ParticleContainerLinkedCellTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/particle/ParticleContainerLinkedCellTest.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/particle/ParticleNeighborsTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/particle/ParticleNeighborsTest.h"

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
gtest_discover_tests(inputReaderTests)
gtest_discover_tests(particleTests)
