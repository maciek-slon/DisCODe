/*!
 * \file LoggerAux.cpp
 * \brief Implementation of common logger functions
 */

#include "LoggerAux.hpp"

namespace Utils {
namespace Logger {

std::string sev2str(Severity sev)
{
	switch(sev) {
	case(Trace):
		return "TRACE";
	case(Debug):
		return "DEBUG";
	case(Info):
		return "INFO";
	case(Notice):
		return "NOTICE";
	case(Warning):
		return "WARNING";
	case(Error):
		return "ERROR";
	case(Critical):
		return "CRITICAL";
	case(Fatal):
		return "FATAL";
	}

	return "UNDEFINED";
}

}
}
