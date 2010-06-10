# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
SET(ptree_files ptree.cpp)

# Create an executable file from sources:
ADD_EXECUTABLE(ptree ${ptree_files})

SET(fs_files fileSearch.cpp)
ADD_EXECUTABLE(fs ${fs_files})
target_link_libraries(fs ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})

ADD_EXECUTABLE(LoggerTest LoggerTest.cpp)
TARGET_LINK_LIBRARIES(LoggerTest ${Boost_THREAD_LIBRARY})

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

ADD_EXECUTABLE(TestOpenCV TestOpenCV.cpp)

MESSAGE(STATUS "OpenCV_LIBRARIES=${OpenCV_LIBRARIES}")

TARGET_LINK_LIBRARIES(TestOpenCV ${OpenCV_LIBRARIES})