#ifndef SYSTEMPROXY_HPP_
#define SYSTEMPROXY_HPP_

#include <string>
#include <map>
#include <vector>

namespace DisCODe {

class Client;

class SystemProxy {
public:
	SystemProxy(Client * client);

	void finish();
private:
	Client * m_client;
};

}

#endif /* SYSTEMPROXY_HPP_ */
