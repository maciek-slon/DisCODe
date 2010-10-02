# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files
FILE(GLOB files *.hpp)

install(FILES ${files}
  DESTINATION include
  COMPONENT headers)
  
# Create library Base
FILE(GLOB files *.cpp)
ADD_LIBRARY(Base SHARED ${files})
TARGET_LINK_LIBRARIES(Base ${DisCODe_LIBRARIES})