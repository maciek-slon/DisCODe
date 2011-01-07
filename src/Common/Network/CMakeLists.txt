# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all header files
FILE(GLOB h_files *.hpp)

INSTALL(FILES ${h_files}
  DESTINATION include/Common/Network COMPONENT sdk
)

# Create library Common
SET(files TCPServer.cpp TCPClient.cpp)
ADD_LIBRARY(Network SHARED ${files})
SET_TARGET_PROPERTIES (Network PROPERTIES DEFINE_SYMBOL  "COMPILING_DLL" )
TARGET_LINK_LIBRARIES(Network ${DisCODe_LIBRARIES})

install(
  TARGETS Network
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)

