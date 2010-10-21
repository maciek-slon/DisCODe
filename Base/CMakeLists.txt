# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files
FILE(GLOB files *.hpp)

install(FILES ${files}
  DESTINATION include/Base
  COMPONENT sdk)
  
FILE(GLOB files Policies/*.hpp)
install(FILES ${files}
  DESTINATION include/Base/Policies
  COMPONENT sdk)
  
# Create library Base
FILE(GLOB files *.cpp)
ADD_LIBRARY(Base SHARED ${files})
TARGET_LINK_LIBRARIES(Base ${DisCODe_LIBRARIES})

INSTALL(TARGETS Base
  RUNTIME
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT sdk)
  
  
INSTALL(TARGETS Base
  RUNTIME
  ARCHIVE DESTINATION bin
  LIBRARY DESTINATION bin
  COMPONENT application)