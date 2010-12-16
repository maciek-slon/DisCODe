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

struct DataBuffer {
	int size;
	char * buf;
};

class TCPServer : public Thread
{
public:
	typedef boost::function<int (const char *, int, char *, int)> service_hook_t;
	typedef boost::function<int (const char *, int)> completion_hook_t;

	TCPServer(int port = 30000, int max_cons = 10, int buffer_size = 20000);

	virtual ~TCPServer();


	void setupHook(service_hook_t h);

	void stop();

protected:
	void run();

	void prepareNewClient(int client_sock);

	void acceptNewClient();

	void handleClient(int i);

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
	service_hook_t m_service_hook;

	///
	completion_hook_t m_completion_hook;

	/// Flag indicating server state
	volatile bool m_running;

	/// Maximum size of buffer for single client
	int m_buffer_size;

	/// Buffers for all clients
	std::map<int, DataBuffer> m_buffers;

	char * m_reply_buffer;

	char * m_tmp_buffer;
};

}

#endif /* TCPSERVER_HPP_ */
