include(FindPkgConfig)

# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

PKG_SEARCH_MODULE(Unicap libunicap)

IF(Unicap_FOUND)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

INCLUDE_DIRECTORIES(${Unicap_INCLUDE_DIRS})

# Create an executable file from sources:
ADD_LIBRARY(srcCameraUniCap SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(srcCameraUniCap ${OpenCV_LIBS} ${DisCODe_LIBRARIES} ${Unicap_LIBRARIES} )

INSTALL_SOURCE(srcCameraUniCap)

ENDIF(Unicap_FOUND)