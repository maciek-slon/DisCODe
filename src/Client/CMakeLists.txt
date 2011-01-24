# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all header files
FILE(GLOB h_files *.hpp)

INSTALL(FILES ${h_files}
  DESTINATION include/Client COMPONENT sdk
)

# Create library Common
FILE(GLOB files *.cpp)
ADD_LIBRARY(DisCODeClient SHARED ${files})
SET_TARGET_PROPERTIES (DisCODeClient PROPERTIES DEFINE_SYMBOL  "COMPILING_DLL" )
TARGET_LINK_LIBRARIES(DisCODeClient ${DisCODe_LIBRARIES})

install(
  TARGETS DisCODeClient
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)

ADD_EXECUTABLE(TestClient ${files})
TARGET_LINK_LIBRARIES(TestClient ${DisCODe_LIBRARIES} Network)

install(
  TARGETS TestClient
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)

SET_TARGET_PROPERTIES(TestClient PROPERTIES
   BUILD_WITH_INSTALL_RPATH ON
   INSTALL_RPATH_USE_LINK_PATH 1
   INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
)
