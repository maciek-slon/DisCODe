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

#SET_TARGET_PROPERTIES(Base
#   PROPERTIES BUILD_WITH_INSTALL_RPATH OFF
#   INSTALL_RPATH .
#)

INSTALL(TARGETS Base
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)
