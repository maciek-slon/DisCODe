/*!
 * \file Logger.cpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */

#include "Logger.hpp"
#include "Console.hpp"

#include <cstdio>

namespace Utils {
namespace Logger {

Logger & Logger::log(const char * file, int line, Severity sev, const std::string & msg) {
	curr_lvl = sev;
	if (sev < level)
		return *this;


	switch (sev) {
	case Trace:
		std::cout << green << "TRACE" << reset << " in " << file << " [" << green << line << reset << "]: ";
		break;
	case Debug:
		std::cout << green << intense << "DEBUG" << reset << " in " << file << " [" << green << line << reset << "]: ";
		break;
	case Info:
		std::cout << cyan << "INFO: " << reset;
		break;
	case Notice:
		std::cout << cyan << intense << "NOTICE: " << reset;
		break;
	case Warning:
		std::cout << yellow << "WARNING: " << reset;
		break;
	case Error:
		std::cout << red << "ERROR: " << reset;
		break;
	case Critical:
		std::cout << red << "CRITICAL: " << reset;
		break;
	case Fatal:
		std::cout << red << intense << "FATAL: " << reset;
		break;
	}

	sum[sev]++;

	return *this;
}

static char char2dump(char ch) {
	if (ch >= 32 && ch <= 127)
		return ch;
	else
		return '.';
}

void Logger::dump(Severity sev, const std::string & msg, void * data, int length) {
	log("", 0, sev, "") << msg << "\n";
	int step;
	int offset;
	char * itr;

	for (itr = (char*)data, offset = 0; length > 0; ) {
		step = length < 16 ? length : 16;
		printf("%04x | ", offset);
		for (int i = 0; i < step; ++i) {
			printf("%02x ", (itr[i]) & 0xFF);
		}

		for (int i = step; i < 16; ++i) {
			printf("   ");
		}

		printf("| ");

		for (int i = 0; i < step; ++i) {
			printf("%c", char2dump(itr[i]));
		}
		printf("\n");
		itr += step;
		offset += step;
		length -= step;
	}
}

} //: namespace Logger
} //: namespace Utils
