/*!
 * \file Task.cpp
 * \brief
 * \date 08-07-2010
 */

#include "Task.hpp"

#include <boost/foreach.hpp>

namespace Core {

bool Task::start() {
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->restart();
	}

	return true;
}

bool Task::stop() {
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->pause();
	}

	return true;
}

bool Task::finish() {
	BOOST_FOREACH(SubtaskPair sp, subtasks) {
		sp.second.finish();
	}


	stop();

	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->finish();
		executor.second->wait(1000);
	}

	return true;
}


} //: namespace Core

