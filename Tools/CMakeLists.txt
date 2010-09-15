#
ADD_EXECUTABLE(xmltree xmltree.cpp)

#
ADD_EXECUTABLE(regexfind regexfind.cpp)
target_link_libraries(regexfind ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})
