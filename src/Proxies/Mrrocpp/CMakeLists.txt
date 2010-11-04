# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
add_library(Mrrocpp SHARED ${files})
target_link_libraries(Mrrocpp ${OpenCV_LIBS} ${DisCODe_LIBRARIES})

INSTALL_PROXY(Mrrocpp)
