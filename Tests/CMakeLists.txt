# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( Boost REQUIRED)

include_directories( ${Boost_INCLUDE_DIRS} )

# Create a variable containing all .cpp files:
set(Tests_SOURCES main.cpp)

# For a large number of source files you can create it in a simpler way
# using file() function:
# file(GLOB Tests_SOURCES *.cpp)

# Create an executable file called helloworld from sources:
add_executable(Tests ${Tests_SOURCES})

# OpenCV + CMake
# Find OpenCV package
FIND_PACKAGE( OpenCV REQUIRED )

# Create executable
ADD_EXECUTABLE( test_opencv test_opencv.cpp )

# Link executable with OpenCV libraries
TARGET_LINK_LIBRARIES( test_opencv ${OpenCV_LIBS} )