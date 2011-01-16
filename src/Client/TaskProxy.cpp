#include "TaskProxy.hpp"
#include "DisCODeClient.hpp"
#include "ExecutorProxy.hpp"

#include <iostream>

#include <boost/foreach.hpp>

namespace DisCODe {

TaskProxy::TaskProxy(Client * client) : m_client(client) {

}

void TaskProxy::start() {
}

void TaskProxy::stop() {
}

int TaskProxy::countExecutors() const {
	return executors.size();
}

ExecutorProxy * TaskProxy::getExecutor(int i) {
	return executors[i];
}

void TaskProxy::refresh() {
	std::string str = m_client->send("listExecutors");

	size_t start = 0, end = 0;
	std::string separator = "\n";

	std::string tmp;

	executors.clear();

	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		if (tmp.empty())
			break;

		ExecutorProxy * ex = new ExecutorProxy(m_client, tmp);
		ex->refresh();

		executors.push_back(ex);

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end > (std::string::npos - separator.size())) ? std::string::npos : end + separator.size());
	}

}

void TaskProxy::print(int indent) {
	for (int i = 0; i < indent; ++i)
		std::cout << "\t";

	std::cout << "Task\n";

	BOOST_FOREACH(ExecutorProxy * ep, executors) {
		ep->print(indent+1);
	}
}

}
