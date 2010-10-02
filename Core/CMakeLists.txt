# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files
FILE(GLOB files *.cpp)

# Create an executable file from sources
ADD_EXECUTABLE(DisCODe ${files})

# Link required libraries
TARGET_LINK_LIBRARIES(DisCODe ${DisCODe_Core_LIBRARIES})

 install(TARGETS DisCODe
   RUNTIME
   DESTINATION bin
   COMPONENT applications)
