/*!
 * \file Task.cpp
 * \brief
 * \date 08-07-2010
 */

#include "Task.hpp"
#include "Executor.hpp"

#include <boost/foreach.hpp>

namespace Core {

Task & Task::operator += (Executor * ex) {
	executors[ex->name()] = ex;
	return *this;
}


Subtask & Task::operator[](const std::string & name) {
	if (subtasks.count(name) < 1) {
		LOG(LWARNING) << "Subtask " << name << " absent. Creating new one...\n";
		subtasks[name] = Subtask(name);
	}

	return subtasks[name];
}

bool Task::start() {
	LOG(LTRACE) << "Starting subtasks...";
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		if (!sp.second.start())
			return false;
	}

	LOG(LTRACE) << "Starting executors...";
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->restart();
	}

	LOG(LTRACE) << "Task started successfully";
	return true;
}

bool Task::stop() {
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->pause();
	}

	return true;
}

bool Task::finish() {
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->finish();
		executor.second->wait(1000);
	}

	Common::Thread::msleep(500);

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
	std::vector<std::string> ret;
	BOOST_FOREACH(ExecutorPair ep, executors) {
		ret.push_back(ep.first);
	}
	return ret;
}


} //: namespace Core

