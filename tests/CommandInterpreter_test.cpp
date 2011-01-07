#define BOOST_TEST_MODULE CommandInterpreter_test
#include <boost/test/included/unit_test.hpp>

#include "CommandInterpreter.hpp"
#include "DisCODeException.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

std::string print(std::vector<std::string> args) {
	std::stringstream ss;
	for (int i = 0; i < args.size(); ++i) {
		ss << args[i] << ",";
	}

	std::cout << ss.str();

	return ss.str();
}

BOOST_AUTO_TEST_CASE( test ) {
	Core::CommandInterpreter interpreter;

	interpreter.addHandler("print", print);
	BOOST_CHECK(interpreter.execute("print:ala:ma:kota") == "ala,ma,kota,");
	//BOOST_CHECK_THROW(interpreter.execute("print3:ala:ma:kota"), Common::DisCODeException);
}
