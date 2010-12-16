#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <boost/function.hpp>

#include "Thread.hpp"

namespace Common {

class TCPServer : public Thread
{
public:
	TCPServer(int port = 30000, int max_cons = 10);

	virtual ~TCPServer();


	void setupHook(boost::function<std::string (const std::string &, int)> h);

	void stop();

protected:
	void run();

private:
	void setNonBlocking(bool b);

	// get sockaddr, IPv4 or IPv6:
	void *get_in_addr(struct sockaddr *sa);


	/// Server socket (one on which it listens for incoming connections)
	int m_sock;

	/// Server address
	sockaddr_in m_addr;

	/// List of all opened sockets (server socketas well as client sockets)
	fd_set m_socks;

	/// Maximum socket descriptor
	int m_maxfd;

	/// Function called when new data comes
	boost::function<std::string (const std::string &, int)> m_service_hook;

	/// Flag indicating server state
	volatile bool m_running;
};

}

#endif /* TCPSERVER_HPP_ */
