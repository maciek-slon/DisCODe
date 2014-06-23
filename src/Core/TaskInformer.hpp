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

		ci.addHandler("start", boost::bind(&TaskInformer::start, this, _1));
		ci.addHandler("stop",  boost::bind(&TaskInformer::stop,  this, _1));

		ci.addHandler("startSubtask",  boost::bind(&TaskInformer::startSubtask,  this, _1));
		ci.addHandler("stopSubtask",  boost::bind(&TaskInformer::stopSubtask,  this, _1));
	}

	/*!
	 * Return list of all executors in current task.
	 */
	std::string listExecutors(std::vector<std::string> /* args */) {
		std::string ret;
		std::vector<std::string> tmp = task.listExecutors();
		BOOST_FOREACH(std::string s, tmp) {
			ret += s + "\n";
		}
		return ret;
	}

	/*!
	 * Return list of all subtasks in current task.
	 */
	std::string listSubtasks(std::vector<std::string> /* args */) {
		std::string ret;
		std::vector<std::string> tmp = task.listSubtasks();
		BOOST_FOREACH(std::string s, tmp) {
			ret += s + "\n";
		}
		return ret;
	}

	/*!
	 * Start whole task
	 */
	std::string stop(std::vector<std::string> /* args */ ) {
		task.stop();
		return "OK";
	}

	/*!
	 * Stop whole task
	 */
	std::string start(std::vector<std::string> /* args */ ) {
		task.start();
		return "OK";
	}

	/*!
	 * Start given subtasks
	 * \arg args list of subtasks to start
	 */
	std::string startSubtask(std::vector<std::string> args ) {
		if (args.size() < 1) {
			LOG(LWARNING) << "TaskInformer::startSubtask: no subtask name provided";
			return "ERR";
		}

		BOOST_FOREACH(std::string & s, args) {
			if (task.checkSubtask(s)) {
				task[s].start();
			} else {
				LOG(LWARNING) << "TaskInformer::startSubtask: subtask " << s << " doesn't exist";
			}
		}

		return "OK";
	}

	/*!
	 * Stop given subtasks
	 * \arg args list of subtasks to start
	 */
	std::string stopSubtask(std::vector<std::string> args ) {
		if (args.size() < 1) {
			LOG(LWARNING) << "TaskInformer::stopSubtask: no subtask name provided";
			return "ERR";
		}

		BOOST_FOREACH(std::string & s, args) {
			if (task.checkSubtask(s)) {
				task[s].stop();
			} else {
				LOG(LWARNING) << "TaskInformer::stopSubtask: subtask " << s << " doesn't exist";
			}
		}

		return "OK";
	}

	std::string subtaskState(std::vector<std::string> args) {
		if (args.size() < 1) {
			LOG(LWARNING) << "TaskInformer::subtaskState: no subtask name provided";
			return "ERR";
		}
		if (!task.checkSubtask(args[0])) {
			LOG(LWARNING) << "TaskInformer::subtaskState: subtask " << args[0] << " doesn't exist";
			return "ERR";
		}
		switch (task[args[0]].state()) {
		case Subtask::Running: return "R";
		case Subtask::Stopped: return "S";
		}
	}

private:
	Core::Task & task;
};

}

#endif /* TASKINFORMER_HPP_ */
