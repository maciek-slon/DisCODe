/*
 * LoggerTest.cpp
 *
 *  Created on: 2010-06-10
 *      Author: mstefanc
 */

#include "../Common/Logger.hpp"

int main() {
	LOG(INFO) << "Info with numbers " << 5 << " and strings " << std::string(":-)") << "\n";
	LOG(WARNING) << "Warning, something painfull but not lethal.\n";
	LOG(ERROR) << "Error, bad bad bad\n";
	LOG(WARNING) << "And warning ince again.\n";
	LOG(FATAL) << "Bye bye :-(\n";

	LOGGER.summary();

	return 0;
}
