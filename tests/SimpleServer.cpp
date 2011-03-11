#include "Network/TCPServer.hpp"

#include <boost/lexical_cast.hpp>
#include <iostream>

int service(const unsigned char * msg, int msg_size, unsigned char * reply, int reply_limit)
{
	std::cout << msg << std::endl;
	reply[0] = 'O';
	reply[1] = 'K';
	reply[2] = '\0';
	return 3;
}

int main()
{
	Common::TCPServer server;
	server.setServiceHook(service);
	server.start();
	return 0;
}
