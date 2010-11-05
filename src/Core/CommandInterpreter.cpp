/*!
 * \file CommandInterpreter.cpp
 * \brief
 * \author mstefanc
 * \date Oct 3, 2010
 */

#include "CommandInterpreter.hpp"

#include "Logger.hpp"

namespace Core {

CommandInterpreter::CommandInterpreter()
{
	// TODO Auto-generated constructor stub

}

CommandInterpreter::~CommandInterpreter()
{
	// TODO Auto-generated destructor stub
}

bool CommandInterpreter::execute(const std::string & cmd)
{
	std::vector<std::string> tokens = parse(cmd);
	if (tokens.empty()) {
		LOG(LERROR) << "Empty string given to execute";
		return false;
	}

	return true;
}

std::vector<std::string> CommandInterpreter::parse(const std::string & cmd)
{
	return std::vector<std::string>();
}

}
