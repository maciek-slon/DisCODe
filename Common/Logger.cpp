/*!
 * \file Logger.cpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */

#include "Logger.hpp"
#include "Console.hpp"

namespace Utils {

Logger & Logger::log(const char * file, int line, Severity sev) {
	switch (sev) {
	case Debug:
		std::cout << green << "DEBUG" << reset << " in " << file << " [" << green << line << reset << "]: ";
		break;
	case Info:
		std::cout << cyan << "INFO: " << reset;
		break;
	case Warning:
		std::cout << yellow << "WARNING: " << reset;
		break;
	case Error:
		std::cout << red << "ERROR: " << reset;
		break;
	case Fatal:
		std::cout << red << intense << "FATAL: " << reset;
		break;
	}

	sum[sev]++;

	return *this;
}

}
