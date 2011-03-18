include(FindPkgConfig)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

PKG_SEARCH_MODULE(SNDFile sndfile)

IF(SNDFile_FOUND)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

INCLUDE_DIRECTORIES(${SNDFile_INCLUDE_DIRS})

# Create an executable file from sources:
ADD_LIBRARY(srcWAVSource SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(srcWAVSource ${OpenCV_LIBS} ${DisCODe_LIBRARIES} ${SNDFile_LIBRARIES} )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

INSTALL_SOURCE(srcWAVSource)

ELSE(SNDFile_FOUND)

MESSAGE(STATUS "SndFile library not found. WAVSource excluded from build")

ENDIF(SNDFile_FOUND)
