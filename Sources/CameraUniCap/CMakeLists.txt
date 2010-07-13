include(FindPkgConfig)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(srcCameraUniCap SHARED ${files})

pkg_search_module(Unicap REQUIRED libunicap)

include_directories(${Unicap_INCLUDE_DIRS})

# Link external libraries
TARGET_LINK_LIBRARIES(srcCameraUniCap ${OpenCV_LIBRARIES} ${FraDIA_LIBRARIES} ${Unicap_LIBRARIES} )

INSTALL_SOURCE(srcCameraUniCap)
