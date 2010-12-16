FIND_PACKAGE( Boost REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# Create an executable file from sources
ADD_EXECUTABLE(CommandInterpreter_test CommandInterpreter_test.cpp ../src/Core/CommandInterpreter.cpp)

ADD_TEST(CommandInterpreter_test ${EXECUTABLE_OUTPUT_PATH}/CommandInterpreter_test)
