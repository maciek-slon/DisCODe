#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <string>

#define BUF_SIZE 1000

namespace Common {

class TCPClient {
public:
	TCPClient();

	~TCPClient();

	bool connect(const std::string & host, const std::string & port);

	std::string recv();
	void send(const std::string & msg);

private:
	int m_sock;

	char buf[BUF_SIZE];

	int sendall(char *buf, int len);
};

}

#endif /* TCPCLIENT_HPP_ */
