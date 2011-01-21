/*!
 * \file CommandInterpreter.hpp
 * \brief Command interpreter.
 * \author mstefanc
 * \date Oct 3, 2010
 */

#ifndef COMMANDINTERPRETER_HPP_
#define COMMANDINTERPRETER_HPP_

#include <vector>
#include <string>
#include <map>

#include <boost/function.hpp>

#include "Informer.hpp"

namespace Core {

struct Command {
	std::string command;
	std::vector<std::string> arguments;

	void print();
};

/*!
 * \brief Command interpreter
 */
class CommandInterpreter
{
public:

	typedef boost::function<std::string(std::vector<std::string>)> handler;

	CommandInterpreter();
	virtual ~CommandInterpreter();

	/*!
	 * Try to parse and execute given command string.
	 *
	 * @param cmd command to execute
	 * @return true on success
	 */
	std::string execute(const std::string & cmd);

	void addHandler(const std::string & cmd, handler h);

	void addInformer(Informer * informer) {
		informer->registerHandlers(*this);
	}

	void printCommands();

protected:
	/*!
	 * Parse given command and return list of tokens.
	 *
	 * @param cmd command to parse
	 * @return list of tokens
	 */
	Command parse(const std::string & cmd);

	typedef std::pair<std::string, handler> handler_pair;

	std::map<std::string, handler > handlers;
};

}

#endif /* COMMANDINTERPRETER_HPP_ */
