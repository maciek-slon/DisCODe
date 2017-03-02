#ifndef DISCODECLIENT_HPP_
#define DISCODECLIENT_HPP_


#include "Network/TCPClient.hpp"

#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace DisCODe {

enum MessageType {

	mtUnknown	= 0,

	mtListExecutors,

	mtListProperties,
	mtSetProperty,
	mtGetProperty
};

class Client {
public:

	typedef boost::function<void(std::vector<std::string>)> list_executors_t;
	typedef boost::function<void(std::string, std::vector<std::string>)> list_components_t;
	typedef boost::function<void(std::string, std::vector<std::string>)> list_properties_t;
	typedef boost::function<void(std::string, std::string, std::string)> get_property_t;

	typedef boost::function<void(void)> connection_lost_t;

	Client(const std::string host = "localhost", const std::string port = "30000") :
		m_host(host),
		m_port(port)
	{
		m_connected = false;
		m_client.setServiceHook(boost::bind(&Client::service, this, _1, _2));
		m_client.setCompletionHook(boost::bind(&Client::check, this, _1, _2));
	}

	std::string send(const std::string & msg) {
		fillBuffer(msg);
		m_client.send(buf, msg.size()+3);
		int ret = m_client.recv(500);
		if (ret < 0) {
			m_connection_lost_handler();
			return std::string();
		}
		std::string rets = std::string((char*)buf);
		//std::cout << "> " << msg << "\n";
		//std::cout << "< " << rets << "\n";
		return rets;
	}

	void setListExecutorsHandler(list_executors_t h) {
		m_list_executors_handler = h;
	}

	void setListComponentsHandler(list_components_t h) {
		m_list_components_handler = h;
	}

	void setListPropertiesHandler(list_properties_t h) {
		m_list_properties_handler = h;
	}

	void setGetPropertyHandler(get_property_t h) {
		m_get_property_handler = h;
	}

	void setConnectionLostHandler(connection_lost_t h) {
		m_connection_lost_handler = h;
	}

	bool connected() const {
		return m_connected;
	}

	const std::string & host() const {
		return m_host;
	}

	const std::string & port() const {
		return m_port;
	}

	bool connect(const std::string host = "localhost", const std::string port = "30000") {
		m_host = host;
		m_port = port;
		m_client.setServiceHook(boost::bind(&Client::service, this, _1, _2));
		m_client.setCompletionHook(boost::bind(&Client::check, this, _1, _2));
		m_connected = m_client.connect(host, port);
		return m_connected;
	}

	bool disconnect() {
		return m_client.disconnect();
	}

protected:
	int check(const unsigned char * msg, int size) {
		return size > 1 ? msg[0] * 256 + msg[1] : 2;
	}

	int service(const unsigned char * msg, int /*size*/) {
		strcpy((char*)buf, (char*)msg+2);

		return 0;
	}

	void fillBuffer(const std::string & msg) {
		buf[0] = (msg.size()+1) / 256;
		buf[1] = (msg.size()+1) % 256;

		strcpy((char*)buf+2, msg.c_str());
		buf[msg.size()+2] = 0;
	}

private:
	Common::TCPClient m_client;

	unsigned char buf[2048];


	list_executors_t m_list_executors_handler;
	list_components_t m_list_components_handler;
	list_properties_t m_list_properties_handler;
	get_property_t m_get_property_handler;
	connection_lost_t m_connection_lost_handler;

	bool m_connected;

	std::string m_host;
	std::string m_port;
};

}

#endif /* DISCODECLIENT_HPP_ */
