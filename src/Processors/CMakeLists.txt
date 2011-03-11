SET(CMAKE_INCLUDE_CURRENT_DIR ON)

macro(ADD_ALL_PROCESSORS)
  FILE(GLOB sub-dirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
#  MESSAGE("Subdirs: ${sub-dirs}")
  foreach(dir ${sub-dirs})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
      ADD_PROCESSOR(${dir})
    endif()
  endforeach()
endmacro()

MESSAGE(STATUS "Building processors:")

ADD_ALL_PROCESSORS()

