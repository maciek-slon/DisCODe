SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

# One line per subfolder
FILE(GLOB local *.cpp)
FILE(GLOB blobs Blobs/*.cpp)

SET(files ${local} ${blobs})

ADD_LIBRARY(Types SHARED ${files})
TARGET_LINK_LIBRARIES(Types ${OpenCV_LIBRARIES})
