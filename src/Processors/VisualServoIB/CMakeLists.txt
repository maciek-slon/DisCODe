# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(DisCODe_LIBRARIES
    ${DisCODe_LIBRARIES}
    ${Boost_SERIALIZATION_LIBRARY}
)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(VisualServoIB SHARED ${files})
TARGET_LINK_LIBRARIES(VisualServoIB ${DisCODe_LIBRARIES})

INSTALL_PROCESSOR(VisualServoIB)
