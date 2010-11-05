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

namespace Core {

/*!
 * \brief Command interpreter
 */
class CommandInterpreter
{
public:
	CommandInterpreter();
	virtual ~CommandInterpreter();

	/*!
	 * Try to parse and execute given command string.
	 *
	 * @param cmd command to execute
	 * @return true on success
	 */
	bool execute(const std::string & cmd);

protected:
	/*!
	 * Parse given command and return list of tokens.
	 *
	 * @param cmd command to parse
	 * @return list of tokens
	 */
	std::vector<std::string> parse(const std::string & cmd);
};

}

#endif /* COMMANDINTERPRETER_HPP_ */
