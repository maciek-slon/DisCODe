FIND_PACKAGE( Boost REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ../src/Common ../src/Base ../src/Core)

# Create an executable file from sources
ADD_EXECUTABLE(CommandInterpreter_test CommandInterpreter_test.cpp ../src/Core/CommandInterpreter.cpp)

ADD_TEST(CommandInterpreter_test ${EXECUTABLE_OUTPUT_PATH}/CommandInterpreter_test)




add_executable(SimpleClient SimpleClient.cpp)
target_link_libraries(SimpleClient Network)

SET_TARGET_PROPERTIES(SimpleClient PROPERTIES
   BUILD_WITH_INSTALL_RPATH ON
   INSTALL_RPATH_USE_LINK_PATH 1
   INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
)

install(
  TARGETS SimpleClient
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)