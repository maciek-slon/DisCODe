include(FindPkgConfig)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

PKG_SEARCH_MODULE(SNDFile sndfile)

PKG_SEARCH_MODULE(PortAudio portaudio)

IF(SNDFile_FOUND AND PortAudio_FOUND)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(Speakers SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(Speakers ${OpenCV_LIBS} ${DisCODe_LIBRARIES}  ${SNDFile_LIBRARIES}  ${PortAudio_LIBRARIES} )

INSTALL_SOURCE(Speakers)

ENDIF(SNDFile_FOUND AND PortAudio_FOUND)
