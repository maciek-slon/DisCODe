# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
add_library(CvVideoWriter SHARED ${files})
target_link_libraries(CvVideoWriter ${OpenCV_LIBS} ${DisCODe_LIBRARIES})

INSTALL_SINK(CvVideoWriter)
