
#
ADD_EXECUTABLE(xmltree xmltree.cpp)
MESSAGE(STATUS "\txmltree")
SET(BUILT_TOOLS ${BUILT_TOOLS} xmltree)

#
ADD_EXECUTABLE(regexfind regexfind.cpp)
target_link_libraries(regexfind ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})
MESSAGE(STATUS "\tregexfind")
SET(BUILT_TOOLS ${BUILT_TOOLS} regexfind)


SET_TARGET_PROPERTIES(${BUILT_TOOLS} PROPERTIES
   BUILD_WITH_INSTALL_RPATH ON
   INSTALL_RPATH_USE_LINK_PATH 1
   INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
)

INSTALL(
  TARGETS ${BUILT_TOOLS}
  RUNTIME DESTINATION bin COMPONENT applications
)



configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dclcreate.py" "${CMAKE_BINARY_DIR}/dclcreate" IMMEDIATE @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/create_component.py" "${CMAKE_BINARY_DIR}/create_component" IMMEDIATE @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/diswizard.py" "${CMAKE_BINARY_DIR}/diswizard" IMMEDIATE @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/diswizard_library.py" "${CMAKE_BINARY_DIR}/diswizard_library.py" IMMEDIATE @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/discode_helper.py" "${CMAKE_BINARY_DIR}/discode_helper.py" IMMEDIATE @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dclmake.py" "${CMAKE_BINARY_DIR}/dclmake" IMMEDIATE @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dclcache.py" "${CMAKE_BINARY_DIR}/dclcache" IMMEDIATE @ONLY)

INSTALL(
  PROGRAMS 
	${CMAKE_BINARY_DIR}/dclcreate 
	${CMAKE_BINARY_DIR}/create_component 
	${CMAKE_BINARY_DIR}/diswizard 
	${CMAKE_BINARY_DIR}/discode_helper.py
	${CMAKE_BINARY_DIR}/dclmake
	${CMAKE_BINARY_DIR}/dclcache
  DESTINATION bin COMPONENT applications
) 

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/diswizard.ui
	${CMAKE_CURRENT_SOURCE_DIR}/diswizard_library.ui
	${CMAKE_CURRENT_SOURCE_DIR}/Library.py
	${CMAKE_BINARY_DIR}/diswizard_library.py
  DESTINATION bin
)
