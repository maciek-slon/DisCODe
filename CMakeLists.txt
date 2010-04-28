# Project name is not mandatory, but you should use it
project(FraDIA)

# States that CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (eg MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${FraDIA_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# The macro below forces the build directory to be different from source directory:
include(MacroOutOfSourceBuild)

macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Required on Windows
if(WIN32)
    SET(BOOST_ROOT D:/boost_1_42_0)
    SET(FraDIA_LIBRARIES xml2 iconv wsock32)
else()
    SET(FraDIA_LIBRARIES xml2 iconv)
endif()

find_package( Boost REQUIRED )

include_directories( . Base Core Common ${Boost_INCLUDE_DIRS})

add_subdirectory(Core)
add_subdirectory(Sources)
add_subdirectory(Processors)
