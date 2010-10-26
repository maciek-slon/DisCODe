SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

# One line per subfolder
FILE(GLOB local *.cpp)
FILE(GLOB blobs Blobs/*.cpp)
FILE(GLOB objects3d Objects3D/*.cpp)

SET(files ${local} ${blobs} ${objects3d})

ADD_LIBRARY(Types SHARED ${files})
TARGET_LINK_LIBRARIES(Types ${OpenCV_LIBRARIES})

SET_TARGET_PROPERTIES(Types
   PROPERTIES BUILD_WITH_INSTALL_RPATH OFF
   INSTALL_RPATH .
)




INSTALL(TARGETS Types
  RUNTIME
  ARCHIVE DESTINATION bin
  LIBRARY DESTINATION bin
  COMPONENT applications)

INSTALL(TARGETS Types
  RUNTIME
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT sdk)


FILE(GLOB local *.hpp)
FILE(GLOB blobs Blobs/*.hpp)
FILE(GLOB objects3d Objects3D/*.hpp)

install(FILES ${local}
  DESTINATION include/Types
  COMPONENT sdk)
  
install(FILES ${blobs}
  DESTINATION include/Types/Blobs
  COMPONENT sdk)
  
install(FILES ${objects3d}
  DESTINATION include/Types/Objects3D
  COMPONENT sdk)
