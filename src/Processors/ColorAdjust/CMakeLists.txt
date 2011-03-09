# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(ColorAdjust SHARED ${files})
TARGET_LINK_LIBRARIES(ColorAdjust ${OpenCV_LIBS} ${DisCODe_LIBRARIES})

INSTALL_PROCESSOR(ColorAdjust)
