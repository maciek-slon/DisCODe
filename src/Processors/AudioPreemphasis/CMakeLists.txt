include(FindPkgConfig)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

PKG_SEARCH_MODULE(SNDFile sndfile)

IF(SNDFile_FOUND)

# Create a variable containing all .cpp files:
FILE(GLOB files *_Processor.cpp)

# Create an executable file from sources:
ADD_LIBRARY(procAudioPreemphasis SHARED ${files})
TARGET_LINK_LIBRARIES(procAudioPreemphasis ${OpenCV_LIBS} ${DisCODe_LIBRARIES} -lsndfile)

INSTALL_PROCESSOR(procAudioPreemphasis)

ELSE(SNDFile_FOUND)

MESSAGE(STATUS "SndFile library not found. AudioPreemphasis excluded from build")

ENDIF(SNDFile_FOUND)
