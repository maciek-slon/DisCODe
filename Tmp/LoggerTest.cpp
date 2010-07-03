/*
 * LoggerTest.cpp
 *
 *  Created on: 2010-06-10
 *      Author: mstefanc
 */

#include "../Common/Logger.hpp"

#include <iostream>

void printMessages() {
	LOG(TRACE) << "Trace\n";
	LOG(DEBUG) << "Debug information\n";
	LOG(INFO) << "Info with numbers " << 5 << " and strings " << std::string(":-)") << "\n";
	LOG(NOTICE) << "Notice this and that!\n";
	LOG(WARNING) << "Warning, something painfull but not lethal.\n";
	LOG(ERROR) << "Error, bad bad bad\n";
	LOG(WARNING) << "And warning ince again.\n";
	LOG(CRITICAL) << "Critical hit!\n";
	LOG(FATAL) << "Bye bye :-(\n";

	LOGGER.summary();
}

int main(int argc, char* argv[]) {

	char str[] = "To jest test dumpowawania różnokształtnych zmiennych\377\177";

	std::cout << "Default debug level (INFO)\n";
	printMessages();

	std::cout << "Setting debug level to TRACE\n";
	LOGGER.setLevel(TRACE);
	printMessages();

	std::cout << "Setting debug level to WARNING\n";
	LOGGER.setLevel(WARNING);
	printMessages();

	std::cout << "Setting debug level to FATAL\n";
	LOGGER.setLevel(FATAL);
	printMessages();

	LOGGER.dump(FATAL, "Dump test", &str, sizeof(str));


	return 0;
}
