#ifndef COMMANDSERVER_HPP_
#define COMMANDSERVER_HPP_

#include "Thread.hpp"
#include "Network/TCPServer.hpp"

#include "CommandInterpreter.hpp"
#include "Task.hpp"

#include <boost/foreach.hpp>

namespace Core {

class TaskInformator {
public:
	TaskInformator(Core::Task & t) : task(t) {

	}

	std::string listExecutors(std::vector<std::string> args) {
		std::string ret;
		std::vector<std::string> tmp = task.listExecutors();
		BOOST_FOREACH(std::string s, tmp) {
			ret += s + "\n";
		}
		return ret;
	}

	std::string listSubtasks(std::vector<std::string> args) {
		std::string ret;
		std::vector<std::string> tmp = task.listSubtasks();
		BOOST_FOREACH(std::string s, tmp) {
			ret += s + "\n";
		}
		return ret;
	}

private:
	Core::Task & task;
};



class CommandServer : public Common::Thread {
public:

	CommandServer(Core::Task & t) : m_informator(t) {
		m_interpreter.addHandler("printExecutors", boost::bind(&TaskInformator::listExecutors, &m_informator, _1));
		m_interpreter.addHandler("printSubtasks",  boost::bind(&TaskInformator::listSubtasks,  &m_informator, _1));

		m_server.setServiceHook(boost::bind(&CommandServer::service, this, _1, _2, _3, _4));
	}

	void stop() {
		m_server.stop();
	}

protected:
	void run() {
		m_server.start();
	}

	int service(const char * msg, int msg_size, char * reply, int reply_limit) {
		std::string rep = m_interpreter.execute(msg+2);

		if (!rep.empty()) {
			//std::cout << "Reply: " << rep << std::endl;
			int rep_size = rep.size() < reply_limit - 3 ? rep.size() + 3 : reply_limit;
			reply[0] = rep_size / 256;
			reply[1] = rep_size % 256;
			strncpy(reply+2, rep.c_str(), reply_limit-2);
			reply[reply_limit-1] = 0;

			/// \todo check, if correct value is returned
			return rep_size;
		} else {
			std::cout << "No reply\n";
			return 0;
		}
	}

	int completion(const char * msg, int msg_size) {
		return msg_size > 1 ? msg[0] * 256 + msg[1] : 2;
	}

private:
	Common::TCPServer m_server;

	TaskInformator m_informator;

	CommandInterpreter m_interpreter;
};

}

#endif /* COMMANDSERVER_HPP_ */
