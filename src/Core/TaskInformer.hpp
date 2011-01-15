#ifndef TASKINFORMER_HPP_
#define TASKINFORMER_HPP_

#include "Informer.hpp"
#include "CommandInterpreter.hpp"

namespace Core {

class TaskInformer : public Informer {
public:
	TaskInformer(Core::Task & t) : task(t) {

	}

	void registerHandlers(CommandInterpreter & ci) {
		ci.addHandler("listExecutors", boost::bind(&TaskInformer::listExecutors, this, _1));
		ci.addHandler("listSubtasks",  boost::bind(&TaskInformer::listSubtasks,  this, _1));
	}

	std::string listExecutors(std::vector<std::string> args) {
		std::string ret;
		std::vector<std::string> tmp = task.listExecutors();
		BOOST_FOREACH(std::string s, tmp) {
			ret += s + "\n";
		}
		return ret;
	}

	std::string listSubtasks(std::vector<std::string> args) {
		std::string ret;
		std::vector<std::string> tmp = task.listSubtasks();
		BOOST_FOREACH(std::string s, tmp) {
			ret += s + "\n";
		}
		return ret;
	}

private:
	Core::Task & task;
};

}

#endif /* TASKINFORMER_HPP_ */
