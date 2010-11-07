/*!
 * \file ScopeLogger.hpp
 * \brief
 * \date 13-07-2010
 */

#ifndef SCOPELOGGER_HPP_
#define SCOPELOGGER_HPP_

#include <string>

#include "LoggerImpl.hpp"

namespace Utils {
namespace Logger {

/*!
 * \class ScopeLogger
 * \brief Small class used to prepare line for logger.
 */
class ScopeLogger
{
public:
	ScopeLogger(Logger & p, const std::string & f, int l, Severity s) : parent(p), file(f), line(l), sev(s) {

	}

	~ScopeLogger() {
		parent.log(file, line, sev, os.str());
	}

	std::ostringstream& get()
	{
		return os;
	}

protected:
	std::ostringstream os;

private:
	ScopeLogger(const ScopeLogger & rhs) : parent(rhs.parent) {}

	ScopeLogger& operator =(const ScopeLogger&) {
		return *this;
	}

private:
	///
	Logger & parent;

	///
	const std::string file;

	///
	int line;

	///
	Severity sev;

};

}
}

#endif /* SCOPELOGGER_HPP_ */
