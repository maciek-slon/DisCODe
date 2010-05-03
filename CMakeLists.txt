# Project name
project(FraDIA)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (eg MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${FraDIA_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# The macro below forces the build directory to be different from source directory:
include(MacroOutOfSourceBuild)

macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

# Define place for all created executables and libraries
SET(LIBRARY_OUTPUT_PATH    ${CMAKE_SOURCE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Required on Windows
if(WIN32)
    SET(BOOST_ROOT D:/boost_1_42_0)
endif()

# Find all required packages
FIND_PACKAGE( Boost REQUIRED thread filesystem system)
FIND_PACKAGE( LibXml2 REQUIRED )

# Make makefiles more verbose
SET(CMAKE_VERBOSE_MAKEFILE ON)

# Set libraries linked with project
SET(FraDIA_LIBRARIES 
    xml2 
    ${Boost_THREAD_LIBRARY} 
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

if(WIN32)
    SET(FraDIA_LIBRARIES ${FraDIA_LIBRARIES} iconv wsock32)    
endif()

# Specify include directories for the build
include_directories( . Base Core Common ${Boost_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} libs/include)

# Specify directories in which the linker will look for libraries
LINK_DIRECTORIES( &{Boost_LIBRARY_DIRS} )

# Add compiler directives
add_definitions(-Wall -pedantic -std=c++0x)

# Add subdirectories to the build - CMake will run CMakeFiles from them
add_subdirectory(Core)
add_subdirectory(Sources)
add_subdirectory(Processors)

# Console commands
IF(WIN32)
    SET(RMRF "rmdir /s /q")
    SET(CP "copy")
ELSE(WIN32)
    SET(RMRF "rm -rf")
    SET(CP "cp")
ENDIF(WIN32)

# =======================================================================
# doxygen support
# =======================================================================

find_package(Doxygen)
if(NOT DOXYGEN_EXECUTABLE)
  message(STATUS "WARNING: Doxygen not found so documentation not generated.")
endif(NOT DOXYGEN_EXECUTABLE)

#
# Generating documentation with doxygen
#
if(DOXYGEN_EXECUTABLE)
  
    # Set html output directory location
    SET(HTML_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/doc/fradia.html)
  
    # N.B. Both the following custom rules assume the doc directory exists
    # at make time, and the following install(DIRECTORY... must have doc exist
    # at cmake time.  Therefore, create the doc directory at CMake time.
    # (Linux experimentation indicates this is a no-op if the empty or
    # non-empty directory already exists.)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)

    ADD_CUSTOM_TARGET(
        doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/FraDIA.Doxyfile
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc_src/fradia.png ${HTML_OUTPUT_DIR}/fradia.png
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/FraDIA.Doxyfile
    )

endif(DOXYGEN_EXECUTABLE)
