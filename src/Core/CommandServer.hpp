#ifndef COMMANDSERVER_HPP_
#define COMMANDSERVER_HPP_

#include "Thread.hpp"
#include "Network/TCPServer.hpp"

#include "CommandInterpreter.hpp"

#include <boost/foreach.hpp>

namespace Core {

class CommandServer : public Common::Thread {
public:

	CommandServer() {
		m_server.setServiceHook(boost::bind(&CommandServer::service, this, _1, _2, _3, _4));
		m_server.setCompletionHook(boost::bind(&CommandServer::completion, this, _1, _2));
	}

	void stop() {
		m_server.stop();
	}

	void addInformer(Informer * informer) {
		m_interpreter.addInformer(informer);
	}

	void printCommands() {
		m_interpreter.printCommands();
	}

protected:
	void run() {
		m_server.start();
	}

	int service(const unsigned char * msg, int msg_size, unsigned char * reply, int reply_limit) {
		std::string rep = m_interpreter.execute((char*)msg+2);

		if (!rep.empty()) {
			std::cout << "Reply: " << rep << std::endl;
			int rep_size = rep.size() < reply_limit - 3 ? rep.size() + 3 : reply_limit;
			reply[0] = rep_size / 256;
			reply[1] = rep_size % 256;
			std::cout << "Reply size: " << rep_size << "=" << (int)reply[0] << "*256+" << (int)reply[1] << std::endl;
			strncpy((char*)reply+2, rep.c_str(), reply_limit-2);
			reply[reply_limit-1] = 0;

			/// \todo check, if correct value is returned
			return rep_size;
		} else {
			std::cout << "No reply\n";
			return 0;
		}
	}

	int completion(const unsigned char * msg, int msg_size) {
		return msg_size > 1 ? msg[0] * 256 + msg[1] : 2;
	}

private:
	Common::TCPServer m_server;

	CommandInterpreter m_interpreter;
};

}

#endif /* COMMANDSERVER_HPP_ */
