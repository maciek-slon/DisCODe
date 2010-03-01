# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
set(CameraOpenCV_SOURCES CameraOpenCV.cpp)

# Create an executable file called helloworld from sources:
add_library(CameraOpenCV SHARED ${CameraOpenCV_SOURCES})

# Link executable with OpenCV libraries
TARGET_LINK_LIBRARIES( CameraOpenCV ${OpenCV_LIBS} )