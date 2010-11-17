/*!
 * \file LoggerOutput.hpp
 * \brief Interface for different logger outputs.
 * \author mstefanc
 * \date 2010-07-13
 */

#ifndef LOGGEROUTPUT_HPP_
#define LOGGEROUTPUT_HPP_

#include "LoggerAux.hpp"
#include "Console.hpp"

#include <string>
#include <cstdio>
#include <sstream>

namespace Utils {
namespace Logger {

/*!
 * \class LoggerOutput
 * \brief Interface for different logger outputs.
 */
class LoggerOutput {
public:
	/*!
	 * \brief Log given message.
	 *
	 * Depending on actual implementation, it can be printen on screen, put to file,
	 * inserted into database etc.
	 *
	 * \param msg message to log
	 * \param sev severity of message
	 * \param file name of file, from which log was called
	 * \param line number of line, from which log was called
	 */
	virtual void print(const std::string & msg, Severity sev, const std::string & file, int line) const = 0;

	void setLvl(Severity sev) {
		lvl = sev;
	}

	Severity getLvl() const {
		return lvl;
	}

private:
	Severity lvl;
};

// =============================================================================
// === Concrete logger outputs
// =============================================================================

class ConsoleOutput : public LoggerOutput {
public:
	void print(const std::string & msg, Severity sev, const std::string & file, int line) const {
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

		std::cout << msg << std::endl;
	}
};

} //: namespace Logger
} //: namespace Utils

#endif /* LOGGEROUTPUT_HPP_ */
