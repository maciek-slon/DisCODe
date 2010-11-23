SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )



SET(WITH_BLOBS ON CACHE BOOL "Build blobs library")
IF(WITH_BLOBS)

    FILE(GLOB blobs Blobs/*.cpp)
    ADD_LIBRARY(TypesBlobs SHARED ${blobs})
    TARGET_LINK_LIBRARIES(TypesBlobs ${OpenCV_LIBRARIES})

    SET_TARGET_PROPERTIES(TypesBlobs
       PROPERTIES BUILD_WITH_INSTALL_RPATH OFF
       INSTALL_RPATH .
    )

    INSTALL(
      TARGETS TypesBlobs
      RUNTIME DESTINATION bin COMPONENT applications
      LIBRARY DESTINATION bin COMPONENT applications
      ARCHIVE DESTINATION lib COMPONENT sdk
    )

    FILE(GLOB blobs Blobs/*.hpp)
    install(
        FILES ${blobs}
        DESTINATION include/Types/Blobs
        COMPONENT sdk
    )

ENDIF(WITH_BLOBS)


# One line per subfolder
FILE(GLOB local *.cpp)
FILE(GLOB objects3d Objects3D/*.cpp)


FILE(GLOB local *.hpp)

FILE(GLOB objects3d Objects3D/*.hpp)

install(
    FILES ${local}
    DESTINATION include/Types
    COMPONENT sdk
)

install(
    FILES ${objects3d}
    DESTINATION include/Types/Objects3D
    COMPONENT sdk
)
