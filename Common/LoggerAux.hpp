/*!
 * \file LoggerAux.hpp
 * \brief
 * \date 13-07-2010
 */

#ifndef LOGGERAUX_HPP_
#define LOGGERAUX_HPP_

#include <string>

namespace Utils {
namespace Logger {

/*!
 * Severity level of message
 */
enum Severity
{
	Trace = 0, ///< Simple information used to trace program execution, prints file name and line
	Debug, ///< Debug message with file and line number
	Info, ///< Information
	Notice, ///< Something more important that simple information
	Warning, ///< Warning, continue execution
	Error, ///< Something bad happened
	Critical, ///< Critical error, there is small chance that execution can be continued
	Fatal
///< Something very bad happened, no chance to continue execution
};

/*!
 * Text representation of severity level
 */
std::string sev2str(Severity sev);

}
}

#endif /* LOGGERAUX_HPP_ */
