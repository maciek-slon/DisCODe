#include "TCPServer.hpp"

#include <boost/lexical_cast.hpp>

std::string service(const std::string & s, int c)
{
	return boost::lexical_cast <std::string>(c) + " : " +  s;
}

int main()
{
	Common::TCPServer server;
	server.setupHook(service);
	server.run();
	return 0;
}
