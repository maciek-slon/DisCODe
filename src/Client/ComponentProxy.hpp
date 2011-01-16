#ifndef COMPONENTPROXY_HPP_
#define COMPONENTPROXY_HPP_

#include <string>
#include <map>
#include <vector>

#include <boost/foreach.hpp>

namespace DisCODe {

class Client;

class ComponentProxy {
public:
	ComponentProxy(Client * client, const std::string & name);

	void refresh();

	void print(int indent = 0);

	int countHandlers() const;

	void triggerHandler(int i);
	void triggerHandler(const std::string & handler);

	std::string name() const {
		return m_name;
	}

private:
	Client * m_client;

	std::string m_name;

	typedef std::pair<std::string, std::string> PropertyPair;

	std::map<std::string, std::string> properties;

	std::vector<std::string> handlers;
};

}

#endif /* COMPONENTPROXY_HPP_ */
