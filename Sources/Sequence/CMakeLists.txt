# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(Sequence SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(Sequence ${OpenCV_LIBS} ${DisCODe_LIBRARIES} )

INSTALL_SOURCE(Sequence)
