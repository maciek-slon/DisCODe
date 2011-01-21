# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenAL library files
#FIND_PACKAGE( OpenAL REQUIRED )

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(WAVSink SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(WAVSink ${OpenCV_LIBS} ${DisCODe_LIBRARIES} -lsndfile)

INSTALL_SOURCE(WAVSink)
