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

	Client(const std::string host = "localhost", const std::string port = "30000") :
		m_host(host),
		m_port(port)
	{
		m_client.setServiceHook(boost::bind(&Client::service, this, _1, _2));
		m_client.setCompletionHook(boost::bind(&Client::check, this, _1, _2));
		m_connected = m_client.connect(host, port);
	}

	std::string send(const std::string & msg) {
		fillBuffer(msg);
		m_client.send(buf, msg.size()+3);
		m_client.recv();
		return std::string((char*)buf);
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

	bool connected() const {
		return m_connected;
	}

	const std::string & host() const {
		return m_host;
	}

	const std::string & port() const {
		return m_port;
	}

protected:
	int check(const unsigned char * msg, int size) {
		std::cout << "Check...\n";
		int ss = msg[0] * 256 + msg[1];
		std::cout << "Computed: " << ss << "(" << (int)msg[0] << "+" << (int)msg[1] << "), got: " << size << std::endl;
		std::cout << msg+2 << std::endl;
		return size > 1 ? msg[0] * 256 + msg[1] : 2;
	}

	int service(const unsigned char * msg, int size) {
		std::cout << "Service... " << size << "\n";
		std::cout << msg+2 << std::endl;

		strcpy((char*)buf, (char*)msg+2);

		return 0;
	}

	void fillBuffer(const std::string & msg) {
		buf[0] = msg.size() / 256;
		buf[1] = msg.size() % 256;

		strcpy((char*)buf+2, msg.c_str());
		buf[msg.size()+2] = 0;
	}

private:
	Common::TCPClient m_client;

	unsigned char buf[1024];


	list_executors_t m_list_executors_handler;
	list_components_t m_list_components_handler;
	list_properties_t m_list_properties_handler;
	get_property_t m_get_property_handler;

	bool m_connected;

	std::string m_host;
	std::string m_port;
};

}

#endif /* DISCODECLIENT_HPP_ */
