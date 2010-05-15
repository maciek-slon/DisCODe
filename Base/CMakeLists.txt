# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files
FILE(GLOB files *.hpp)

install(FILES ${files}
  DESTINATION include
  COMPONENT headers)