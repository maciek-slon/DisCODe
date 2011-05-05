#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <string>

#include <boost/function.hpp>

namespace Common {

class TCPClient {
public:
	/// Function descriptor of service hook
	typedef boost::function<int (const unsigned char *, int)> service_hook_t;

	/// Function descriptor of completion hook
	typedef boost::function<int (const unsigned char *, int)> completion_hook_t;

	TCPClient(int buffer_size = 200000);

	~TCPClient();

	/*!
	 * Connect to remote server
	 *
	 * @param host host name
	 * @param port port number
	 *
	 * @return true on success, false otherwise
	 */
	bool connect(const std::string & host, const std::string & port);

	/*!
	 * Receive next packet.
	 *
	 * Method receives data from socket until whole packet is collected (m_completion_hook is used
	 * to determine packet completness) or until given timeout expires.
	 *
	 * @param msec_timeout
	 *
	 * @return
	 */
	int recv(int msec_timeout = -1);

	/*!
	 * Send data to socket.
	 *
	 * @param msg data to send
	 * @param size data size
	 *
	 * @return number of bytes sent (equal to size on success)
	 */
	int send(const unsigned char * msg, int size);

	/*!
	 * Set function called when new packet arrives.
	 */
	void setServiceHook(service_hook_t h);

	/*!
	 *
	 * @param h
	 */
	void setCompletionHook(completion_hook_t h);

private:
	/// Client socket
	int m_sock;

	/// Buffer size
	int m_buffer_size;

	/// Data buffer
	unsigned char * m_buf;
	unsigned char * m_tmp_buf;

	/// Size of current buffer data
	int m_size;

	/// Function called when new data comes
	service_hook_t m_service_hook;

	/// Function called to determine incoming packet completness
	completion_hook_t m_completion_hook;
};

}

#endif /* TCPCLIENT_HPP_ */
