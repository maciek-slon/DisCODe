#include "TaskProxy.hpp"
#include "DisCODeClient.hpp"
#include "ExecutorProxy.hpp"

#include <iostream>

#include <boost/foreach.hpp>

namespace DisCODe {

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

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

	start = 0;
	end = 0;
	str = m_client->send("listSubtasks");
	subtasks.clear();
	while (end != std::string::npos) {
		end = str.find(separator, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		tmp = str.substr(start, (end == std::string::npos) ? std::string::npos : end - start);

		trim(tmp);
		if (tmp.empty())
			break;

		subtasks.push_back(tmp);

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

int TaskProxy::countSubtasks() const {
	return subtasks.size();
}

std::string TaskProxy::getSubtask(int i) {
	return subtasks[i];
}

TaskProxy::State TaskProxy::state() {
	std::string str = m_client->send("taskState");
	if (str == "I") return TaskProxy::Initializing;
	if (str == "R") return TaskProxy::Running;
	if (str == "S") return TaskProxy::Stopped;
	
	return TaskProxy::Initializing;
}

TaskProxy::State TaskProxy::subtaskState(const std::string & name) {
	std::string msg = "subtaskState:";
	msg += name;
	std::string str = m_client->send(msg);
	if (str == "I") return TaskProxy::Initializing;
	if (str == "R") return TaskProxy::Running;
	if (str == "S") return TaskProxy::Stopped;

	return TaskProxy::Initializing;
}

bool TaskProxy::startSubtask(const std::string & name) {
	std::string msg = "startSubtask:";
	msg += name;
	std::string str = m_client->send(msg);

	return (str == "OK");
}

bool TaskProxy::stopSubtask(const std::string & name) {
	std::string msg = "stopSubtask:";
	msg += name;
	std::string str = m_client->send(msg);

	return (str == "OK");
}

}
