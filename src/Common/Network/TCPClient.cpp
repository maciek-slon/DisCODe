#include "TCPClient.hpp"

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

TCPClient::TCPClient()
{

}

TCPClient::~TCPClient()
{

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

std::string TCPClient::recv()
{
	int size, recvd;
	recvd = ::recv(m_sock, buf, BUF_SIZE, NULL);

	size = buf[0] + buf[1] * 256;
	buf[recvd] = 0;

	if (recvd != size + 2) {
		std::cout << "Sie zepsulo\n";
	}

	return std::string(buf + 2);
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
