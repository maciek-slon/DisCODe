/*!
 * \file Socket.cpp
 * \brief Class responsible for communication through sockets.
 * - methods definition
 * \date 18.03.2008
 */

#include <stdexcept>
#include <limits>
#include <cmath>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include "Socket.hpp"
#include "Logger.hpp"

namespace Proxies {

namespace Mrrocpp {

using namespace std;

Socket::Socket() :
	fd(-1)
{

}

Socket::~Socket()
{
	closeSocket();
}

void Socket::setupServerSocket(int port)
{
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw runtime_error("socket() failed: " + string(strerror(errno)));
	}

	int on = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) < 0) {
		throw runtime_error("setsockopt() failed: " + string(strerror(errno)));
	}
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(int))) {
		throw std::runtime_error("setsockopt(): " + std::string(strerror(errno)));
	}

	sockaddr_in m_addr;
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons(port);

	if (::bind(fd, (struct sockaddr*) &m_addr, sizeof(m_addr)) < 0) {
		throw runtime_error("bind() failed: " + string(strerror(errno)));
	}

	if (listen(fd, 1) < 0) {
		throw runtime_error("listen() failed: " + string(strerror(errno)));
	}
}

boost::shared_ptr <Proxies::Mrrocpp::Socket> Socket::acceptConnection()
{
	sockaddr_in m_addr;
	int addr_length = sizeof(m_addr);

	int acceptedFd = accept(fd, (sockaddr *) &m_addr, (socklen_t *) &addr_length);

	if (acceptedFd < 0) {
		throw runtime_error("accept() failed: " + string(strerror(errno)));
	}
	boost::shared_ptr <Socket> s = boost::shared_ptr <Socket>(new Socket());
	s->fd = acceptedFd;

	return s;
}

bool Socket::isDataAvailable(double sec)
{
	//	log_dbg("Socket::isDataAvailable()\n");
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	struct timeval tv;
	struct timeval *tvPtr = NULL;
	if (sec != numeric_limits <double>::infinity()) {
		tv.tv_sec = floor(sec);
		tv.tv_usec = round(fmod(sec, 1) * 1e6);
		tvPtr = &tv;
	}

	int retval = select(fd + 1, &rfds, NULL, NULL, tvPtr);

	if (retval < 0) {
		throw runtime_error("select() failed: " + string(strerror(errno)));
	}
	return retval > 0;
}

bool Socket::isSocketOpened() const
{
	return fd >= 0;
}

void Socket::closeSocket()
{
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
}

int Socket::getFd()
{
	return fd;
}

void Socket::writev2(const void *buf1, size_t buf1Size, const void *buf2, size_t buf2Size)
{
	struct iovec iov[2];
	ssize_t nwritten;

	iov[0].iov_base = (void*) buf1;
	iov[0].iov_len = buf1Size;
	iov[1].iov_base = (void*) buf2;
	iov[1].iov_len = buf2Size;

	nwritten = writev(fd, iov, 2);
	if (nwritten == -1) {
		throw runtime_error("Socket::writev2() nwritten == -1");
	}
	if ((size_t) nwritten != buf1Size + buf2Size) {
		throw runtime_error("Socket::writev2() nwritten != buf1Size + buf2Size");
	}
}

void Socket::read(void *buf, size_t bufSize)
{
	int r;

	r = ::read(fd, buf, bufSize);
	if (r == -1) {
		throw runtime_error("Socket::read: r == -1");
	}
	if ((size_t) r != bufSize) {
		throw runtime_error("Socket::read: r != bufSize");
	}
}

} // namespace Mrrocpp {

} // namespace Proxies
