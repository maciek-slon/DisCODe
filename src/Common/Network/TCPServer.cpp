#include "TCPServer.hpp"

#include <fcntl.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

namespace Common {

TCPServer::TCPServer(int port, int max_cons)
{
	memset(&m_addr, 0, sizeof(m_addr));

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1) {
		std::cout << "Socket creation failed." << std::endl;
		return;
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons(port);

	int bind_return = bind(m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));

	if (bind_return == -1) {
		std::cout << "Bind failed." << std::endl;
		return;
	}

	int listen_return = listen(m_sock, max_cons);

	if (listen_return == -1) {
		std::cout << "Listen failed." << std::endl;
		return;
	}

	//setNonBlocking(true);

	FD_SET(m_sock, &m_socks);
	m_maxfd = m_sock;
}

TCPServer::~TCPServer()
{
	if (m_sock != -1)
		close( m_sock);
}

void TCPServer::run()
{
	int addr_length = sizeof(m_addr);
	int newfd;

	struct timeval tv;
	fd_set read_fds;

	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	int nbytes;

	char buf[256];
	char remoteIP[INET6_ADDRSTRLEN];

	m_running = true;

	while (m_running) {
		tv.tv_sec = 0;
		tv.tv_usec = 250000;
		read_fds = m_socks;

		int select_return = select(m_maxfd + 1, &read_fds, NULL, NULL, &tv);

		if (select_return == -1) {
			std::cout << "Select failed!" << std::endl;
			return;
		}
		if (select_return == 0) {
			//std::cout << "Select timed out." << std::endl;
			continue;
		}

		// run through the existing connections looking for data to read
		for (int i = 0; i <= m_maxfd; i++) {
			if (FD_ISSET(i, &read_fds)) { // we got one!!
				// check, if server listening socket is ready
				if (i == m_sock) {
					std::cout << "Accept..." << std::endl;
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(m_sock, (struct sockaddr *) &remoteaddr, &addrlen);
					if (newfd == -1) {
						std::cout << "Accept failed." << std::endl;
						continue;
					} else {
						FD_SET(newfd, &m_socks); // add to master set
						if (newfd > m_maxfd) {
							// keep track of the max
							m_maxfd = newfd;
						}

						std::cout << "New connection from "
								<< inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*) &remoteaddr), remoteIP, INET6_ADDRSTRLEN)
								<< " on " << newfd << std::endl;
					}
				} else {
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
						// got error or connection closed by client
						if (nbytes == 0) {
							// connection closed
							std::cout << "Socket " << i << " hung up" << std::endl;
						} else {
							std::cout << "recv failed!" << std::endl;
							continue;
						}
						close(i); // bye!
						FD_CLR(i, &m_socks); // remove from master set
					} else {
						// we got some data from a client
						buf[nbytes] = 0;
						std::string query = buf+2;
						std::cout << "Received: " << buf+2 << std::endl;
						query = m_service_hook(query, i);
						buf[0] = query.size() & 0xFF;
						buf[1] = (query.size() >> 8) & 0xFF;
						memcpy(buf+2, query.c_str(), query.size());
						send(i, buf, query.length()+2, MSG_NOSIGNAL);
					}
				} // END handle data from client
			} // END got new incoming connection

		}
	}

}

void TCPServer::stop() {
	m_running = false;
}

void TCPServer::setupHook(boost::function <std::string(const std::string &, int)> h)
{
	m_service_hook = h;
}

void TCPServer::setNonBlocking(bool b)
{
	int opts;

	opts = fcntl(m_sock, F_GETFL);

	if (opts < 0) {
		return;
	}

	if (b)
		opts = (opts | O_NONBLOCK);
	else
		opts = (opts & ~O_NONBLOCK);

	fcntl(m_sock, F_SETFL, opts);
}

// get sockaddr, IPv4 or IPv6:
void *TCPServer::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	return NULL;
}

}
