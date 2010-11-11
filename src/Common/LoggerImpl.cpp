/*!
 * \file LoggerImpl.cpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */

#include "LoggerImpl.hpp"

#include <cstdio>
#include <cctype>

#include <boost/foreach.hpp>

namespace Utils {
namespace Logger {

//Logger* Logger::inst = NULL;

Logger::~Logger() {
}

Logger & Logger::log(const std::string & file, int line, Severity sev, const std::string & msg) {
	BOOST_FOREACH(LoggerOutput & output, outputs) {
		if (sev < output.getLvl())
			continue;

		output.print(msg, sev, file, line);
	}

	sum[sev]++;

	return *this;
}

static char char2dump(char ch) {
	return (std::isprint(ch) ? ch : '.');
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
