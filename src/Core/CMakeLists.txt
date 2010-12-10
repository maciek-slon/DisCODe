# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files
FILE(GLOB files *.cpp)

# Create an executable file from sources
ADD_EXECUTABLE(discode ${files})

# Link required libraries
TARGET_LINK_LIBRARIES(discode ${DisCODe_Core_LIBRARIES})

SET_TARGET_PROPERTIES(discode PROPERTIES
   BUILD_WITH_INSTALL_RPATH ON
   INSTALL_RPATH_USE_LINK_PATH 1
   INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
)

install(
  TARGETS discode
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib
)
