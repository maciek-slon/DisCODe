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
	stop();

	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->finish();
	}

	return true;
}


} //: namespace Core

