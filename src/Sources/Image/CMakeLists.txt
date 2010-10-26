# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(srcImage SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(srcImage ${DisCODe_LIBRARIES})

INSTALL_SOURCE(srcImage)
