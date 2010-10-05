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
project(DisCODe)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (eg MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${DisCODe_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# The macro below forces the build directory to be different from source directory:
include(MacroOutOfSourceBuild)

macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

# Include ADD_PROCESSOR and ADD_SOURCE macros
include(AddModules)

# Checking compiler flags
include(CheckCXXCompilerFlag)

# Define place for all created executables and libraries
SET(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Required on Windows
if(WIN32)
    SET(BOOST_ROOT D:/boost_1_42_0)
endif(WIN32)

SET(Boost_DEBUG FALSE)

# Find all required packages
FIND_PACKAGE( Boost REQUIRED thread filesystem system regex program_options)

# Make makefiles more verbose
SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Set libraries linked with project
SET(DisCODe_LIBRARIES
    ${Boost_THREAD_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# Specify include directories for the build
include_directories( . Base Core Common Types ${Boost_INCLUDE_DIR})

# Specify directories in which the linker will look for libraries
LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )

# Add compiler directives
add_definitions(-Wall)

# Checking if compiler supports C++0x standard
CHECK_CXX_COMPILER_FLAG(-std=c++0x CPP0x)

# If C++0x is supported then use it
if(CPP0x)
    MESSAGE(STATUS "c++0x supported")
    add_definitions(-std=c++0x)
else(CPP0x)
    MESSAGE(STATUS "c++0x NOT supported")
endif(CPP0x)


set(WITH_UNIFIED_TIMER OFF CACHE BOOL "Use unified timer (based on ctime)")

IF(WIN32)
    SET(DisCODe_LIBRARIES ${DisCODe_LIBRARIES} )
    ADD_DEFINITIONS(-DWIN32)
ELSE(WIN32)
    SET(DisCODe_LIBRARIES ${DisCODe_LIBRARIES} rt dl)
ENDIF(WIN32)

IF(WITH_UNIFIED_TIMER)
    add_definitions(-DUNIFIED_TIMER)
ENDIF(WITH_UNIFIED_TIMER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fPIC")

# Create Common library
add_subdirectory(Common)
# Use created library
SET(DisCODe_LIBRARIES ${DisCODe_LIBRARIES} Common)

add_subdirectory(Base)
SET(DisCODe_LIBRARIES ${DisCODe_LIBRARIES} Base)

SET(DisCODe_Core_LIBRARIES ${DisCODe_LIBRARIES})

# Create Types library
add_subdirectory(Types)

# Use created library
SET(DisCODe_LIBRARIES ${DisCODe_LIBRARIES} Types)

# Add subdirectories to the build - CMake will run CMakeFiles from them
add_subdirectory(Core)
add_subdirectory(Sources)
add_subdirectory(Processors)
add_subdirectory(Sinks)

# Small utility programs
set(WITH_TOOLS ON CACHE BOOL "Build tools")

IF (WITH_TOOLS)
    MESSAGE(STATUS "Building tools")
    add_subdirectory(Tools)
ENDIF(WITH_TOOLS)

# Small test programs
set(WITH_TMP_PROGRAMS OFF CACHE BOOL "Build temporary programs")

IF (WITH_TMP_PROGRAMS)
    MESSAGE(STATUS "Building temporary programs")
    add_subdirectory(Tmp)
ENDIF(WITH_TMP_PROGRAMS)


# Create documentation
add_subdirectory(doc_src)

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
