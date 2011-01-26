/*!
 * \file Socket.hpp
 * \brief Class responsible for communication through sockets.
 * - class declaration
 * \date 18.03.2008
 */

#ifndef _SOCKET_
#define _SOCKET_

#include <sys/socket.h>
#include <string.h>

#include <boost/shared_ptr.hpp>

namespace Proxies {

namespace Mrrocpp {

/*!
 * \class Socket
 * \brief Class responsible for communication through sockets.
 */
class Socket
{
public:
	Socket();
	virtual ~Socket();

	/**
	 * Call socket, bind, listen
	 * @param port
	 */
	void setupServerSocket(int port);

	/**
	 * Return accepted connection.
	 * @return
	 */
	boost::shared_ptr <Socket> acceptConnection();

	void closeSocket();

	/**
	 * Calls select(fd, usec) to check if there is data to read.
	 * @param sec Time in seconds. Passing 0 will make it return immediately.
	 * Passing std::numeric_limits\<double\>::infinity() will make it block.
	 * @return true when data is available.
	 */
	bool isDataAvailable(double sec = 0);

	bool isSocketOpened() const;

	int getFd();

	void writev2(const void *buf1, size_t buf1Size, const void *buf2, size_t buf2Size);

	void read(void *buf, size_t bufSize);

private:
	/** Socket file descriptor. */
	int fd;
};

} // namespace Mrrocpp {

} // namespace Proxies

#endif
