/*!
 * \file LoggerImpl.hpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */
#ifndef LOGGERIMPL_HPP_
#define LOGGERIMPL_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Singleton.hpp"
#include "LoggerAux.hpp"
#include "LoggerOutput.hpp"

namespace Utils {
namespace Logger {

/*!
 * \class Logger
 * \brief Class for loggind messages.
 *
 * Example usage of this class is available in \ref using_logger.
 */
class Logger: public Base::Singleton <Logger>
{
	/*!
	 * Singleton class must be a friend, because only it can call protected constructor.
	 */
	friend class Base::Singleton <Logger>;

public:

	virtual ~Logger();

	/*!
	 * Start message. Prints severity info and - if required - file name and line number
	 * from where printing is called.
	 */
	Logger & log(const std::string & file, int line, Severity sev, const std::string & msg);

	/*!
	 * Template stream operator used for printing any type of data.
	 */
	template <class T>
	Logger & operator<<(const T & data)
	{
		if (curr_lvl >= level)
			print(data);
		return *this;
	}

	/*!
	 * Template method used to print any kind of data.
	 *
	 * To print custom types you can either overload operator<< in that class
	 * or specialize this method.
	 */
	template <class T>
	void print(const T & data)
	{
		std::cout << data;
	}

	/*!
	 * Set logging severity level.
	 *
	 * All messages that has severity below set level are ignored. At default level
	 * is set to NOTICE.
	 */
	void setLevel(Severity lvl)
	{
		level = lvl;
	}

	/*!
	 * Binary dump
	 */
	void dump(Severity sev, const std::string & msg, void * data, int length);

	/*!
	 * Print out summary (number of warnings, errors etc).
	 */
	void summary()
	{
		std::cout << sum[Trace] << " traces\n" << sum[Debug] << " debugs\n" << sum[Info] << " informations\n"
				<< sum[Notice] << " notices\n" << sum[Warning] << " warnings\n" << sum[Error] << " errors\n"
				<< sum[Critical] << " critical errors\n" << sum[Fatal] << " fatal errors\n";
	}

	/*!
	 * Add new logger output.
	 */
	void addOutput(LoggerOutput * out, Severity lvl) {
		out->setLvl(lvl);
		outputs.push_back(out);
	}

protected:
	Logger()
	{
		sum[0] = sum[1] = sum[2] = sum[3] = sum[4] = sum[5] = sum[6] = sum[7] = sum[8] = sum[9] = 0;

		level = Notice;
	}

	Logger(const Logger &)
	{
	}

	/// sum of messages of each type
	int sum[10];

	/// current logging level
	int level;

	/// level of actually printed message
	int curr_lvl;

	std::vector<LoggerOutput *> outputs;
};

}
}

#endif /* LOGGERIMPL_HPP_ */
