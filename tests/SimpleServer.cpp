#include "Network/TCPServer.hpp"

#include <boost/lexical_cast.hpp>
#include <iostream>

int service(const char * msg, int msg_size, char * reply, int reply_limit)
{
	std::cout << msg << std::endl;
	return 0;
}

int main()
{
	Common::TCPServer server;
	server.setupHook(service);
	server.start();
	while(1) {
		sleep(1);
	}
	return 0;
}
