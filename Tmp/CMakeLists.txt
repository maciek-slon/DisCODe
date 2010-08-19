include(FindPkgConfig)

#
ADD_EXECUTABLE(LoggerTest LoggerTest.cpp)
TARGET_LINK_LIBRARIES(LoggerTest ${Boost_THREAD_LIBRARY} Common)

#
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE(TestOpenCV TestOpenCV.cpp)
MESSAGE(STATUS "OpenCV_LIBRARIES=${OpenCV_LIBRARIES}")
TARGET_LINK_LIBRARIES(TestOpenCV ${OpenCV_LIBRARIES} rt)

#
pkg_search_module(Unicap REQUIRED libunicapgtk)
MESSAGE(STATUS "Unicap: ${Unicap_INCLUDE_DIRS}")
include_directories(${Unicap_INCLUDE_DIRS})
ADD_EXECUTABLE(TestUnicap TestUnicap.cpp)
TARGET_LINK_LIBRARIES(TestUnicap ${Unicap_LIBRARIES})

#
ADD_EXECUTABLE(TestV4L TestV4L.cpp)
TARGET_LINK_LIBRARIES(TestV4L ${OpenCV_LIBRARIES})

#
ADD_EXECUTABLE(PolicyTest PolicyTest.cpp)

#
ADD_EXECUTABLE(OpenCVSpeed OpenCVSpeed.cpp)
TARGET_LINK_LIBRARIES(OpenCVSpeed ${OpenCV_LIBRARIES})

ADD_EXECUTABLE(ptree ptree.cpp)
TARGET_LINK_LIBRARIES(ptree ${OpenCV_LIBRARIES})
