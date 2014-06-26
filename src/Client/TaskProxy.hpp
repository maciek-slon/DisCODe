#ifndef TASKPROXY_HPP_
#define TASKPROXY_HPP_

#include <map>
#include <string>
#include <vector>

namespace DisCODe {

class Client;
class ExecutorProxy;

class TaskProxy {
public:
	typedef enum {Initializing, Running, Stopped} State;

	TaskProxy(Client * client);

	void start();
	void stop();

	ExecutorProxy * getExecutor(int i);

	void refresh();

	void print(int indent = 0);

	int countExecutors() const;
	
	int countSubtasks() const;
	std::string getSubtask(int i);

	State state();

	State subtaskState(const std::string & name);

	bool startSubtask(const std::string & name);
	bool stopSubtask(const std::string & name);

private:
	Client * m_client;

	typedef std::pair<std::string, ExecutorProxy *> ExecutorPair;

	std::vector<ExecutorProxy *> executors;
	std::vector<std::string> subtasks;
};

}

#endif /* TASKPROXY_HPP_ */
