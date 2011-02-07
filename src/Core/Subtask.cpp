/*!
 * \file Subtask.cpp
 * \brief
 * \date 08-07-2010
 */

#include "Subtask.hpp"
#include "Component.hpp"
#include "Logger.hpp"
#include "Executor.hpp"

#include <boost/foreach.hpp>

namespace Core {


Subtask::~Subtask()
{

}

void Subtask::stop()
{
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->pause();
	}
}

bool Subtask::start()
{
	bool ret = true;
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->restart();
	}
	return ret;
}

void Subtask::initialize()
{
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->initialize();
	}
}

void Subtask::finish()
{
	BOOST_FOREACH(ExecutorPair executor, executors) {
		executor.second->finish();
	}
}

Subtask & Subtask::operator += (Executor * ex) {
	executors[ex->name()] = ex;
	return *this;
}

std::vector<std::string> Subtask::listExecutors() {
	std::vector<std::string> ret;
	BOOST_FOREACH(ExecutorPair ep, executors) {
		ret.push_back(ep.first);
	}
	return ret;
}

}
