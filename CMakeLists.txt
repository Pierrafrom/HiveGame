cmake_minimum_required(VERSION 3.10)
project(HiveGame)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory
include_directories(includes)

# Add source files for the main executable
file(GLOB_RECURSE SOURCES
        src/models/*.cpp
)

# Add header files (for IDEs, mainly)
file(GLOB_RECURSE HEADERS
        includes/models/*.h
)

# Create the main executable
add_executable(HiveGame ${SOURCES})

# Add Google Test
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Define the test executable (without src/main.cpp)
file(GLOB_RECURSE TEST_SOURCES
        tests/models/*.cpp
        tests/main.cpp
)
add_executable(HiveGameTests ${TEST_SOURCES} ${SOURCES})

# Link Google Test to the test executable
target_link_libraries(HiveGameTests gtest gtest_main)

# Enable CTest for test discovery and running
enable_testing()
add_test(NAME AllTests COMMAND HiveGameTests)

# Include GoogleTest module for automatic test discovery
include(GoogleTest)
gtest_discover_tests(HiveGameTests)
