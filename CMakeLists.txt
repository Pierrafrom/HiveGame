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
        src/controllers/*.cpp
        src/utils/*.cpp
        src/main.cpp
)

# Add header files (for IDEs, mainly)
file(GLOB_RECURSE HEADERS
        includes/models/*.h
        includes/controllers/*.h
        includes/utils/*.h
        includes/config/*.h
)

# Create the main executable
add_executable(HiveGame ${SOURCES}
        src/models/strategies/MoveStrategy.cpp)

# Add Google Test
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Define the test sources without src/main.cpp
file(GLOB_RECURSE TEST_SOURCES
        tests/models/*.cpp
        tests/main.cpp
        src/models/*.cpp
        src/controllers/*.cpp
        src/utils/*.cpp
)

# Remove src/main.cpp from TEST_SOURCES to avoid multiple main definitions
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Create the test executable
add_executable(HiveGameTests ${TEST_SOURCES}
        tests/models/PlayerTest.cpp
        tests/models/PieceTest.cpp
        tests/models/StrategyTest.cpp)

# Link Google Test to the test executable
target_link_libraries(HiveGameTests gtest gtest_main)

# Enable CTest for test discovery and running
enable_testing()
add_test(NAME AllTests COMMAND HiveGameTests)

# Include GoogleTest module for automatic test discovery
include(GoogleTest)
gtest_discover_tests(HiveGameTests)
