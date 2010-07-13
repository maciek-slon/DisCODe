/*!
 * \file ScopeLogger.hpp
 * \brief
 * \date 13-07-2010
 */

#ifndef SCOPELOGGER_HPP_
#define SCOPELOGGER_HPP_

#include "Logger.hpp"

namespace Utils {
namespace Logger {

/*!
 * \class ScopeLogger
 * \brief Small class used to prepare line for logger.
 */
class ScopeLogger
{
public:
	ScopeLogger(Logger & p) : parent(p) {

	}

	~ScopeLogger() {
		parent.log(file, line, lvl, msg);
	}

	std::ostringstream& get(const char * f, int l, Severity s)
	{
		file = f;
		line = l;
		sev = s;
		return os;
	}

protected:
	std::ostringstream os;

private:
	ScopeLogger(const ScopeLogger&) {}

	ScopeLogger& operator =(const ScopeLogger&) {
		return *this;
	}

private:
	///
	Severity sev;

	///
	std::string & file;

	///
	int line;

	///
	Logger & parent;
};

}
}

#endif /* SCOPELOGGER_HPP_ */
