/*!
 * \file LoggerImpl.cpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */

#include "LoggerImpl.hpp"

#include <cstdio>

namespace Utils {
namespace Logger {

Logger* Logger::inst = NULL;

Logger::~Logger() {
	for (size_t i = 0; i < outputs.size(); ++i) {
		delete outputs[i];
	}
}

Logger & Logger::log(const std::string & file, int line, Severity sev, const std::string & msg) {
	for (size_t i = 0; i < outputs.size(); ++i) {
		if (sev < outputs[i]->getLvl())
			continue;

		outputs[i]->print(msg, sev, file, line);
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

void Logger::addOutput(LoggerOutput * out, Severity lvl) {
	out->setLvl(lvl);
	outputs.push_back(out);
}


} //: namespace Logger
} //: namespace Utils
