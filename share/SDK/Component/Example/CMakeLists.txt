# =[ General project info ]==============================================
project(Example)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include/Base)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include/Common)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include/Types)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../bin)

# =[ Search for required libraries]======================================

# Boost
SET(BOOST_INCLUDEDIR $ENV{BASE_PATH}/libs/boost/include)
MESSAGE(STATUS ${BOOST_INCLUDEDIR})

SET(Boost_DEBUG FALSE)
FIND_PACKAGE( Boost REQUIRED thread filesystem system regex program_options)

SET(LIBS
    ${Boost_THREAD_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    Base
    Common
)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Add another libraries here (for example when you need OpenCV):
# Find OpenCV library files
# FIND_PACKAGE( OpenCV REQUIRED )
# SET(LIBS ${OpenCV_LIBS} ${OpenCV_LIBRARIES} ${LIBS} )

# =[ Prepare files to build ]============================================
# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources
ADD_LIBRARY(Example SHARED ${files})

# Link with libraries
TARGET_LINK_LIBRARIES(Example ${LIBS})
