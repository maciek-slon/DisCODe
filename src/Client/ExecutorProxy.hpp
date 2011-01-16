#ifndef EXECUTORPROXY_HPP_
#define EXECUTORPROXY_HPP_

#include <string>
#include <map>
#include <vector>

namespace DisCODe {

class Client;
class ComponentProxy;

class ExecutorProxy {
public:
	ExecutorProxy(Client * client, const std::string & name);

	void refresh();

	void print(int indent = 0);

	std::string name() const {
		return m_name;
	}

	int countComponents() const;

	ComponentProxy * getComponent(int i);

private:
	Client * m_client;

	std::string m_name;

	std::vector<ComponentProxy *> components;
};

}

#endif /* EXECUTORPROXY_HPP_ */
