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

add_subdirectory(Tests)