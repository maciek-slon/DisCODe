# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Find OpenCV library files
FIND_PACKAGE( OpenAL REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(SoundFile SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(SoundFile ${OpenCV_LIBS} ${DisCODe_LIBRARIES} ${OpenAL_LIBS})

INSTALL_SOURCE(SoundFile)
