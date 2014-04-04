/*!
 * \file Task.cpp
 * \brief
 * \date 08-07-2010
 */

#include "Task.hpp"
#include "Executor.hpp"
#include "Logger.hpp"

#include <boost/foreach.hpp>

namespace Core {

Subtask & Task::operator[](const std::string & name) {
	if (subtasks.count(name) < 1) {
		LOG(LDEBUG) << "Subtask " << name << " absent. Creating new one...";
		subtasks[name] = Subtask(name);
	}

	return subtasks[name];
}

bool Task::start() {
	LOG(LTRACE) << "Starting subtasks...";
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		sp.second.start();
	}

	LOG(LTRACE) << "Task started successfully";
	return true;
}

bool Task::stop() {
	LOG(LTRACE) << "Stopping subtasks...";
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		sp.second.stop();
	}
	LOG(LTRACE) << "Task stopped.";

	return true;
}

void Task::initialize() {
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		sp.second.initialize();
	}
}

bool Task::finish() {
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		sp.second.stop();
		sp.second.finish();
	}

	return true;
}

std::vector<std::string> Task::listSubtasks() {
	std::vector<std::string> ret;
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		ret.push_back(sp.first);
	}
	return ret;
}

std::vector<std::string> Task::listExecutors() {
	std::vector<std::string> ret, tmp;
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		tmp = sp.second.listExecutors();
		ret.insert(ret.end(), tmp.begin(), tmp.end());
	}
	return ret;
}

} //: namespace Core

