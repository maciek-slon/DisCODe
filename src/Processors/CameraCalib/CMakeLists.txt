# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
add_library(procCameraCalib SHARED ${files})
target_link_libraries(procCameraCalib ${OpenCV_LIBS} ${DisCODe_LIBRARIES})

INSTALL_PROCESSOR(procCameraCalib)
