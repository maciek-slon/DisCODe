# =[ FraDIA ]============================================================
#
# CMake Build Rules for FraDIA
#
# =[ License ]===========================================================
#
# License information
#
# =[ CMake basic usage ]=================================================
#
# Basic Usage:
#
# For more information about CMake, see http://www.cmake.org
#
# =======================================================================

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
SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Set libraries linked with project
SET(FraDIA_LIBRARIES
    xml2
    ${Boost_THREAD_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

# Specify include directories for the build
include_directories( . Base Core Common ${Boost_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})

# Specify directories in which the linker will look for libraries
LINK_DIRECTORIES( &{Boost_LIBRARY_DIRS} )

# Add compiler directives
add_definitions(-Wall -pedantic -std=c++0x)

set(WITH_UNIFIED_TIMER OFF CACHE BOOL "Use unified timer (based on ctime)")
set(WITH_STH OFF CACHE BOOL "Something")

IF(WIN32)
    SET(FraDIA_LIBRARIES ${FraDIA_LIBRARIES} iconv wsock32 dld)
    add_definitions(-DWIN32)
ELSE(WIN32)
    SET(FraDIA_LIBRARIES ${FraDIA_LIBRARIES} rt)
ENDIF(WIN32)

IF(WITH_UNIFIED_TIMER)
    add_definitions(-DINIFIED_TIMER)
ENDIF(WITH_UNIFIED_TIMER)

# Add subdirectories to the build - CMake will run CMakeFiles from them
add_subdirectory(Core)
add_subdirectory(Base)
add_subdirectory(Common)
add_subdirectory(Sources)
add_subdirectory(Processors)


# =======================================================================
# Doxygen support
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


# =======================================================================
# CPack: binary installer generator
# =======================================================================

#
# CPack variables used when generating binary installer
#
set(CPACK_PACKAGE_NAME "FraDIA")
set(CPACK_PACKAGE_VENDOR "FraDIA")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FraDIA - Framework for Digital Image Analysis")
set(CPACK_PACKAGE_VERSION "2.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "FraDIA")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\icon.bmp")

# License file
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# Tell CPack all of the components to install. The "ALL"
# refers to the fact that this is the set of components that
# will be included when CPack is instructed to put everything
# into the binary installer (the default behavior).
set(CPACK_COMPONENTS_ALL applications sources processors headers)

# Applications component contains main FraDIA executable and libraries
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "FraDIA Executable")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "Framework for Digital Image Analysis")
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")

# Sources component contains all data sources
set(CPACK_COMPONENT_SOURCES_DISPLAY_NAME "Data sources")
set(CPACK_COMPONENT_SOURCES_DESCRIPTION "Data sources description")
set(CPACK_COMPONENT_SOURCES_GROUP "Runtime")
set(CPACK_COMPONENT_SOURCES_DEPENDS applications)

# Processors component contains all data processors
set(CPACK_COMPONENT_PROCESSORS_DISPLAY_NAME "Data processors")
set(CPACK_COMPONENT_PROCESSORS_DESCRIPTION "Data processors desctiption")
set(CPACK_COMPONENT_PROCESSORS_GROUP "Runtime")
set(CPACK_COMPONENT_PROCESSORS_DEPENDS applications)

# Headers component contains all files necessary to develop
# additional FraDIA modules (data sources and processors)
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "Development Headers")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "All files necessary to develop additional components (data sources and processors)")
set(CPACK_COMPONENT_HEADERS_GROUP "SDK")

# This must always be last!
include(CPack)