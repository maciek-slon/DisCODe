# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

LINK_DIRECTORIES(/opt/AVT_GigE_SDK/lib-pc/x64/4.4/)
include_directories(/opt/AVT_GigE_SDK/inc-pc)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(CameraGigE SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(CameraGigE ${OpenCV_LIBS} ${DisCODe_LIBRARIES} libPvAPI.so )

INSTALL_SOURCE(CameraGigE)
