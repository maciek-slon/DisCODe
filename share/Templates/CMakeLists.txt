# Project name
PROJECT(TEMPLATE_ARG_DCL_NAME)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Find DisCODe installation
FIND_PACKAGE(DisCODe REQUIRED)

###########################################
# Additional libraries
###########################################

FIND_PACKAGE(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Uncomment following lines if OpenCV is used
# Find OpenCV library files
# FIND_PACKAGE(OpenCV REQUIRED)


# Use CMake macros provided with DisCODe
set(CMAKE_MODULE_PATH ${DisCODe_DIR}/share/DisCODe ${CMAKE_MODULE_PATH})
include(DisCODeMacros)

# Set default install prefix to DisCODe location
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
      ${DisCODe_DIR} CACHE PATH "${CMAKE_PROJECT_NAME} install prefix" FORCE
  )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Add all components here