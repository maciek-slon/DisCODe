#include "ExecutorProxy.hpp"
#include "DisCODeClient.hpp"
#include "ComponentProxy.hpp"

#include <boost/foreach.hpp>

namespace DisCODe {

ExecutorProxy::ExecutorProxy(Client * client, const std::string & name) : m_client(client), m_name(name) {

}

int ExecutorProxy::countComponents() const {
	return components.size();
}

ComponentProxy * ExecutorProxy::getComponent(int i) {
	return components[i];
}

void ExecutorProxy::refresh() {
	std::string request;
	request += "listComponents";
	request += ":";
	request += m_name;

	std::string str = m_client->send(request);

	size_t start = 0, end = 0;
	std::string separator = "\n";

	std::string tmp;

	components.clear();

	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		ComponentProxy * cp = new ComponentProxy(m_client, tmp);
		cp->refresh();
		components.push_back(cp);

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}
}

void ExecutorProxy::print(int indent) {
	for (int i = 0; i < indent; ++i)
		std::cout << "\t";

	std::cout << "[Ex] " << m_name << "\n";

	BOOST_FOREACH(ComponentProxy * cp, components) {
		cp->print(indent+1);
	}
}

}
