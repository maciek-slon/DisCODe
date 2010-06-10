# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all header files
FILE(GLOB h_files *.hpp)

INSTALL(FILES ${h_files}
  DESTINATION include
  COMPONENT headers)
  
# Create library Common
FILE(GLOB files *.cpp)
ADD_LIBRARY(Common STATIC ${files})
TARGET_LINK_LIBRARIES(Common ${FraDIA_LIBRARIES})