/*!
 * \file Logger.hpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */
#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <string>

#include "Singleton.hpp"

namespace Utils {

/*!
 * \class Logger
 * \brief
 */
class Logger : public Base::Singleton <Logger>
{
	/*!
	 * Singleton class must be a friend, because only it can call protected constructor.
	 */
	friend class Base::Singleton <Logger>;

public:

	/*!
	 * Severity level of message
	 */
	enum Severity {
		Info = 0, ///< Information
		Warning,  ///< Warning, continue execution
		Error,    ///< Something bad happened
		Fatal,    ///< Something very bad happened, no chance to continue execution
		Debug     ///< Debug message with file and line number
	};

	virtual ~Logger() {};

	/*!
	 * Start message. Prints severity info and - if required - file name and line number
	 * from where printing is called.
	 */
	Logger & log(const char * file, int line, Severity sev);

	/*!
	 * Template stream operator used for printing any type of data.
	 */
	template<class T>
	Logger & operator<<(const T & data) {
		std::cout << data;
		return *this;
	}

	/*!
	 * Print out summary (number of warnings, errors etc).
	 */
	void summary() {
		std::cout <<
				sum[Info] << " informations\n" <<
				sum[Warning] << " warnings\n" <<
				sum[Error] << " errors\n" <<
				sum[Fatal] << " fatal errors\n";
	}

protected:
	Logger() {
		sum[0] = sum[1] = sum[2] = sum[3] = 0;
	}

	Logger(const Logger &) {}

	/// sum of messages of each type
	int sum[10];
};


#define LOGGER Utils::Logger::instance()

#define INFO     Utils::Logger::Info
#define WARNING  Utils::Logger::Warning
#define ERROR    Utils::Logger::Error
#define FATAL    Utils::Logger::Fatal
#define DEBUG    Utils::Logger::Debug

/// Start message printing
#define LOG(level) (Utils::Logger::instance().log(__FILE__, __LINE__, level))

}

#endif /* LOGGER_HPP_ */
