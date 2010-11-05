# =[ General project info ]==============================================
project(AudioFile_Source)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include/Base)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include/Common)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include/Types)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../lib)

# =[ Search for required libraries]======================================

# Boost
SET(Boost_DEBUG FALSE)
FIND_PACKAGE( Boost REQUIRED thread filesystem system regex program_options)

SET(LIBS
    ${Boost_THREAD_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# Add another libraries here (for example when you need OpenCV):
# Find OpenCV library files
 FIND_PACKAGE( OpenAL REQUIRED )
 SET(LIBS ${OpenAL_LIBS} ${OpenAL_LIBRARIES} ${LIBS} )

# =[ Prepare files to build ]============================================
# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources
ADD_LIBRARY(AudioFile_Source SHARED ${files})

# Link with libraries
TARGET_LINK_LIBRARIES(AudioFile_Source ${LIBS})
