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

	int countHandlers() const {
		return handlers.size();
	}

	std::string getHandler(int i) {
		return handlers[i];
	}

	void triggerHandler(int i);
	void triggerHandler(const std::string & handler);




	int countProperties() const {
		return properties.size();
	}

	std::string getPropertyName(int i) {
		return properties[i];
	}

	std::string getPropertyValue(int i);
	std::string setPropertyValue(int i, const std::string & val);
	std::string setPropertyValue(const std::string & name, const std::string & val);

	std::string getPropertyType(int i);
	std::string getPropertyToolTip(int i);

	std::vector<std::string> getPropertyConstraints(int i) {
		return constraints[i];
	}

	std::string name() const {
		return m_name;
	}

private:
	Client * m_client;

	std::string m_name;

	//typedef std::pair<std::string, std::string> PropertyPair;

	//std::map<std::string, std::string> properties;

	std::vector<std::string> properties;

	std::vector<std::string> handlers;

	void refreshPropertyConstraints(int i);

	std::vector<std::vector<std::string> > constraints;
};

}

#endif /* COMPONENTPROXY_HPP_ */
