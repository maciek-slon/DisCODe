/*!
 * \file ExecutorManager.hpp
 * \brief File containing the ExecutorManager class.
 *
 * \author mstefanc
 * \date 2010-06-23
 */

#ifndef EXECUTORMANAGER_HPP_
#define EXECUTORMANAGER_HPP_

#include "Executor.hpp"
#include "Logger.hpp"
#include "DisCODeException.hpp"

#include <map>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

namespace Core {

class ExecutorManager {
	std::map<std::string, Executor*> executors;
	typedef std::pair<std::string, Executor*> exp;

public:
	ExecutorManager() {
	}

	~ExecutorManager() {

	}

	void release() {
		BOOST_FOREACH(exp e, executors)
			delete e.second;
	}

	Executor * createExecutor(const std::string & name) {
		if (executors.count(name) > 0) {
			LOG(LWARNING) << "Executor " << name << " already created. Returning previous one.\n";
			return executors[name];
		}

		Executor * ex = new Executor(name);

		executors[name] = ex;
		LOG(LINFO) << name << " executor created.\n";

		return ex;
	}

	Executor * getExecutor(const std::string & name) {
		if (executors.count(name) < 1) {
			LOG(LERROR) << "Executor " << name << " can't be found!";
			throw Common::DisCODeException("getExecutor");
		}

		return executors[name];
	}

private:
	std::vector<std::string> types;
};

} //: namespace Core

#endif /* EXECUTOR */
