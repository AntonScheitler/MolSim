file(GLOB_RECURSE TESTFILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(mytests ${TESTFILES})
target_link_libraries(mytests
    src
    gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(mytests)
