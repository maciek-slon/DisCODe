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
	
	State state();

private:
	Client * m_client;

	typedef std::pair<std::string, ExecutorProxy *> ExecutorPair;

	std::vector<ExecutorProxy *> executors;
};

}

#endif /* TASKPROXY_HPP_ */
