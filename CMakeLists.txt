cmake_minimum_required(VERSION 3.10)
project(HiveGame)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(includes)

# Include user-specific configurations
include(cmake/UserConfig.cmake OPTIONAL)

# Check if QT_DIR is set for Qt mode
if(ENABLE_GUI AND NOT DEFINED QT_DIR)
    message(FATAL_ERROR "QT_DIR is not defined. Please set it in 'cmake/UserConfig.cmake'.")
endif()

# Find Qt if GUI is enabled
if(ENABLE_GUI)
    set(CMAKE_PREFIX_PATH ${QT_DIR})
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
endif()

# Add source files recursively, excluding main files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/models/*.cpp
        ${CMAKE_SOURCE_DIR}/src/controllers/*.cpp
        ${CMAKE_SOURCE_DIR}/src/utils/*.cpp
)

# Add the appropriate main file based on the mode
if(ENABLE_GUI)
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/src/main_gui.cpp)
else()
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/src/main_cli.cpp)
endif()

# Ensure that SOURCES is not empty
if(SOURCES STREQUAL "")
    message(FATAL_ERROR "No source files found. Ensure your src directory has the required files.")
endif()

# Create the main executable
add_executable(HiveGame ${SOURCES})

# Link Qt libraries if in GUI mode
if(ENABLE_GUI)
    target_link_libraries(HiveGame Qt6::Core Qt6::Widgets Qt6::Gui)
endif()

# Add Google Test
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Define test sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/tests/models/*.cpp
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

# Include game sources in the test target
list(APPEND TEST_SOURCES ${SOURCES})

# Remove main files from test sources to avoid multiple definitions of main()
list(REMOVE_ITEM TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/src/main_cli.cpp
        ${CMAKE_SOURCE_DIR}/src/main_gui.cpp
)

# Create the test executable
add_executable(HiveGameTests ${TEST_SOURCES})

# Link Google Test and Qt libraries to the test executable
target_link_libraries(HiveGameTests gtest gtest_main)
if(ENABLE_GUI)
    target_link_libraries(HiveGameTests Qt6::Core Qt6::Widgets Qt6::Gui)
endif()

# Enable CTest for test discovery and running
enable_testing()
add_test(NAME AllTests COMMAND HiveGameTests)

# Include GoogleTest module for automatic test discovery
include(GoogleTest)
gtest_discover_tests(HiveGameTests)
