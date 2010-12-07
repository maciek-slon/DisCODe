# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all header files
FILE(GLOB h_files *.hpp)

INSTALL(FILES ${h_files}
  DESTINATION include/Common COMPONENT sdk
)

# Create library Common
FILE(GLOB files *.cpp)
ADD_LIBRARY(Common SHARED ${files})
SET_TARGET_PROPERTIES (Common PROPERTIES DEFINE_SYMBOL  "COMPILING_DLL" )
TARGET_LINK_LIBRARIES(Common ${DisCODe_LIBRARIES})

install(
  TARGETS Common
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)

