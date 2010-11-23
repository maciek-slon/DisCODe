# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *_Processor.cpp)

# Create an executable file from sources:
ADD_LIBRARY(procAudioSpectrogram SHARED ${files})
TARGET_LINK_LIBRARIES(procAudioSpectrogram ${OpenCV_LIBS} ${DisCODe_LIBRARIES} -lsndfile)

INSTALL_PROCESSOR(procAudioSpectrogram)
