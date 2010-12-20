#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <string>

#include <boost/function.hpp>

namespace Common {

class TCPClient {
public:
	/// Function descriptor of completion hook
	typedef boost::function<int (const char *, int)> completion_hook_t;

	TCPClient(int buffer_size = 20000);

	~TCPClient();

	bool connect(const std::string & host, const std::string & port);

	int recv(char * buf, int to_recv, int msec_timeout = -1);
	int send(const char * msg, int size);

private:
	int m_sock;

	int m_buffer_size;
	char * m_buf;
	int m_size;

	completion_hook_t m_completion_hook;
};

}

#endif /* TCPCLIENT_HPP_ */
