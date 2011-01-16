#include "ComponentProxy.hpp"
#include "DisCODeClient.hpp"

namespace DisCODe {

ComponentProxy::ComponentProxy(Client * client, const std::string & name) : m_client(client), m_name(name) {

}

void ComponentProxy::refresh() {
	std::string request;
	request += "listProperties";
	request += ":";
	request += m_name;

	std::string str = m_client->send(request);

	size_t start = 0, end = 0;
	std::string separator = "\n";

	std::string tmp;

	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		if (!properties.count(tmp))
			properties[tmp] = "";

		request = "getProperty";
		request += ":";
		request += m_name;
		request += ":";
		request += tmp;

		std::string val = m_client->send(request);
		properties[tmp] = val;

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}



	request = "listHandlers";
	request += ":";
	request += m_name;

	str = m_client->send(request);

	start = 0, end = 0;
	separator = "\n";

	handlers.clear();
	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		handlers.push_back(tmp);


		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}
}

void ComponentProxy::print(int indent) {
	std::string pref;
	for (int i = 0; i < indent; ++i)
		pref += "\t";

	std::cout << pref << "[C] " << m_name << "\n";

	pref += "\t";

	BOOST_FOREACH(PropertyPair pp, properties) {
		std::cout << pref << "[P] " << pp.first << "=" << pp.second << std::endl;
	}

	BOOST_FOREACH(std::string h, handlers) {
		std::cout << pref << "[H] " << h << std::endl;
	}
}

void ComponentProxy::triggerHandler(const std::string & handler) {
	for (int i = 0; i < handlers.size(); ++i) {
		if (handlers[i] == handler) {
			triggerHandler(i);
			break;
		}
	}
}

void ComponentProxy::triggerHandler(int i) {
	std::string req;

	req += "triggerHandler:";
	req += m_name + ":";
	req += handlers[i];

	m_client->send(req);
}

}
