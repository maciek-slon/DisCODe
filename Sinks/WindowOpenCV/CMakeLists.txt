# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

# Create an executable file called helloworld from sources:
add_library(WindowOpenCV SHARED WindowOpenCV.cpp)

# Link executable with OpenCV libraries
TARGET_LINK_LIBRARIES( WindowOpenCV ${OpenCV_LIBS} )