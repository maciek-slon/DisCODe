# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(srcCameraV4L SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(srcCameraV4L ${OpenCV_LIBRARIES} ${DisCODe_LIBRARIES})

INSTALL_SOURCE(srcCameraV4L)
