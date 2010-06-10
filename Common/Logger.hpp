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

	enum Severity {
		Info = 0,
		Warning,
		Error,
		Fatal
	};

	virtual ~Logger() {};


	Logger & log(const char * file, int line, Severity sev) {
		sum[sev]++;
		std::cout << sev2str(sev) << " in " << file << " [" << line << "]: ";
		return *this;
	}

	template<class T>
	Logger & operator<<(const T & data) {
		std::cout << data;
		return *this;
	}

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


	std::string sev2str(Severity sev) {
		switch (sev) {
		case Info:
			return "INFO";
		case Warning:
			return "WARNING";
		case Error:
			return "ERROR";
		case Fatal:
			return "FATAL";
		default:
			return "UNDEFINED";
		}
	}

	int sum[4];
};

#define LOGGER Utils::Logger::instance()

#define INFO     Utils::Logger::Info
#define WARNING  Utils::Logger::Warning
#define ERROR    Utils::Logger::Error
#define FATAL    Utils::Logger::Fatal

#define LOG(level) (Utils::Logger::instance().log(__FILE__, __LINE__, level))

}

#endif /* LOGGER_HPP_ */
