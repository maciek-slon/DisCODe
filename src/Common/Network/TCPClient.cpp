#include "TCPClient.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstring>
#include <iostream>

namespace Common {

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

TCPClient::TCPClient(int buffer_size) : m_sock(-1), m_buffer_size(buffer_size), m_buf(NULL), m_size(0)
{
	m_buf = new char[m_buffer_size];
}

TCPClient::~TCPClient()
{
	close(m_sock);
	delete [] m_buf;
}

bool TCPClient::connect(const std::string & host, const std::string & port)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		std::cout << "getaddrinfo: " << gai_strerror(rv) << std::endl;
		return false;
	}

	// loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((m_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		if (::connect(m_sock, p->ai_addr, p->ai_addrlen) == -1) {
			close(m_sock);
			perror("client: connect");
			continue;
		}
		break;
	}
	if (p == NULL) {
		std::cout << "client: failed to connect\n";
		return false;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s, sizeof s);
	std::cout << "client: connecting to " << s << std::endl;

	freeaddrinfo(servinfo); // all done with this structure
	return true;
}

int TCPClient::recv(char * buf, int buf_size, int msec_timeout)
{
	/// \todo Block on select instead of recv.
	int recvd = 0, res;

	struct timeval tv, ttv;
	if (msec_timeout >= 0) {
		tv.tv_sec = msec_timeout / 1000;
		tv.tv_usec = msec_timeout % 1000 * 1000;
	}

	while (recvd < buf_size) {
		res = ::recv(m_sock, buf+recvd, buf_size-recvd, NULL);
		if (res < 0) {
			// recv error
			break;
		} else if (!res) {
			// disconnected
			break;
		} else {
			// received some data
			recvd += res;
			if (recvd >= buf_size)
				break;
		}
	}

	return recvd;
}

int TCPClient::send(const char * msg, int size)
{
	int total = 0;
	// how many bytes we've sent
	int bytesleft = size; // how many we have left to send
	int n;

	while (total < size) {
		n = ::send(m_sock, msg + total, bytesleft, 0);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}
	return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

}
