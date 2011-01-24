/*!
 * \file CommandInterpreter.cpp
 * \brief
 * \author mstefanc
 * \date Oct 3, 2010
 */

#include "CommandInterpreter.hpp"

#include <vector>
#include <iostream>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "DisCODeException.hpp"

#include "Logger.hpp"

namespace Core {

void Command::print() {
	std::cout << command << "(";
	BOOST_FOREACH(std::string s, arguments) {
		std::cout << s << ",";
	}
	std::cout << ")" << std::endl;
}




CommandInterpreter::CommandInterpreter()
{
	// TODO Auto-generated constructor stub

}

CommandInterpreter::~CommandInterpreter()
{
	// TODO Auto-generated destructor stub
}

std::string CommandInterpreter::execute(const std::string & str)
{
	Command cmd;

	LOG(LINFO) << "Executing " << str;

	try {
		cmd = parse(str);
	} catch(...) {
		throw;
	}

	if (handlers.count(cmd.command)) {
		return handlers[cmd.command](cmd.arguments);
	}

	LOG(LWARNING) << cmd.command << " has no assigned handler.";
	//throw Common::DisCODeException(cmd.command + " has no assigned handler.");
	return "Unknown command";
}

Command CommandInterpreter::parse(const std::string & str)
{
	Command cmd;

	size_t start = 0, end = 0;
	std::string separator = ":";

	bool cmd_found = false;

	std::string tmp;

	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (!cmd_found) {
			cmd_found = true;
			cmd.command = tmp;
		} else {
			cmd.arguments.push_back(tmp);
		}

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}

	return cmd;
}

void CommandInterpreter::addHandler(const std::string & cmd, handler h) {
	handlers[cmd] = h;
}


void CommandInterpreter::printCommands() {
	LOG(LINFO) << "Registered commands:";
	BOOST_FOREACH(handler_pair hp, handlers) {
		LOG(LINFO) << "\t" << hp.first;
	}
}


}
