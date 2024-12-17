cmake_minimum_required(VERSION 3.16) # Qt6 nécessite souvent une version plus récente
project(HiveGame)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define GUI mode option
option(ENABLE_GUI "Enable GUI mode" ON)

# Include directories
include_directories(includes)

if(ENABLE_GUI)
    include_directories(includes/controllers/gui)
    include_directories(includes/views/gui)
else()
    include_directories(includes/controllers/cli)
    include_directories(includes/views/cli)
endif()

# Include user-specific configurations
include(cmake/UserConfig.cmake OPTIONAL)

# Check if QT_DIR is set for Qt mode
if(ENABLE_GUI AND NOT DEFINED QT_DIR)
    message(FATAL_ERROR "QT_DIR is not defined. Please set it in 'cmake/UserConfig.cmake'.")
endif()

# Find Qt if GUI is enabled
if(ENABLE_GUI)
    set(CMAKE_PREFIX_PATH ${QT_DIR})
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
endif()

# Add model sources
file(GLOB_RECURSE MODEL_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/models/*.cpp
)

# Add controller and view sources based on the mode
if(ENABLE_GUI)
    file(GLOB_RECURSE CONTROLLER_SOURCES CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/controllers/gui/*.cpp
    )
    file(GLOB_RECURSE VIEW_SOURCES CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/views/gui/*.cpp
    )
    set(MAIN_SOURCE ${CMAKE_SOURCE_DIR}/src/main_gui.cpp)
else()
    file(GLOB_RECURSE CONTROLLER_SOURCES CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/controllers/cli/*.cpp
    )
    file(GLOB_RECURSE VIEW_SOURCES CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/views/cli/*.cpp
    )
    set(MAIN_SOURCE ${CMAKE_SOURCE_DIR}/src/main_cli.cpp)
endif()

# Gather all sources
set(SOURCES ${MODEL_SOURCES} ${CONTROLLER_SOURCES} ${VIEW_SOURCES} ${MAIN_SOURCE})

# Process headers with Q_OBJECT
if(ENABLE_GUI)
    file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/views/gui/*.h
            ${CMAKE_SOURCE_DIR}/src/controllers/gui/*.h
    )
    qt6_wrap_cpp(MOC_SOURCES ${HEADERS}) # Generate moc files for headers containing Q_OBJECT
endif()

# Ensure that SOURCES is not empty
if(SOURCES STREQUAL "")
    message(FATAL_ERROR "No source files found. Ensure your src directory has the required files.")
endif()

# Create the main executable
add_executable(HiveGame ${SOURCES} ${MOC_SOURCES})

# Include termcolor and nlohmann/json for colored output and JSON parsing
target_include_directories(HiveGame PRIVATE ${CMAKE_SOURCE_DIR}/external/termcolor ${CMAKE_SOURCE_DIR}/external)

# Link Qt libraries if in GUI mode
if(ENABLE_GUI)
    target_link_libraries(HiveGame Qt6::Core Qt6::Gui Qt6::Widgets)
endif()

# Add Google Test
if(EXISTS ${CMAKE_SOURCE_DIR}/external/googletest)
    add_subdirectory(external/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
else()
    message(FATAL_ERROR "GoogleTest not found in 'external/googletest'. Please add it.")
endif()

# Define test sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/tests/models/*.cpp
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

# Include game sources in the test target (excluding main files)
list(APPEND TEST_SOURCES ${MODEL_SOURCES} ${CONTROLLER_SOURCES} ${VIEW_SOURCES})
list(REMOVE_ITEM TEST_SOURCES ${MAIN_SOURCE})

# Create the test executable
add_executable(HiveGameTests ${TEST_SOURCES})

# Include termcolor and nlohmann/json for tests
target_include_directories(HiveGameTests PRIVATE ${CMAKE_SOURCE_DIR}/external/termcolor ${CMAKE_SOURCE_DIR}/external)

# Link Google Test and Qt libraries to the test executable
target_link_libraries(HiveGameTests gtest gtest_main)
if(ENABLE_GUI)
    target_link_libraries(HiveGameTests Qt6::Core Qt6::Gui Qt6::Widgets)
endif()

# Enable CTest for test discovery and running
enable_testing()
add_test(NAME AllTests COMMAND HiveGameTests)

# Include GoogleTest module for automatic test discovery
include(GoogleTest)
gtest_discover_tests(HiveGameTests)
