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

#include <map>
#include <string>
#include <vector>

namespace Core {

class ExecutorManager {
	std::map<std::string, Executor*> executors;

public:
	ExecutorManager() {
	}

	Executor * createExecutor(const std::string & name, const std::string & type) {
		if (executors.count(name) > 0) {
			LOG(WARNING) << "Executor " << name << " already created. Returning previous one.\n";
			return executors[name];
		}

		Executor * ex;
		if (type == "continous") {
			ex = new ContinousExecutor(name);
		} else if (type == "passive") {
			ex = new PassiveExecutor(name);
		} else if (type == "periodic") {
			ex = new PeriodicExecutor(name);
		} else {
			LOG(ERROR) << "Executor type " << type << " not allowed!\n";
			throw Common::FraDIAException("createExecutor");
		}

		executors[name] = ex;
		LOG(INFO) << name << " (" << type << ") executor created.\n";

		return ex;
	}

	Executor * getExecutor(const std::string & name) {
		if (executors.count(name) < 1) {
			LOG(ERROR) << "Executor " << name << " can't be found!";
			throw Common::FraDIAException("getExecutor");
		}

		return executors[name];
	}
};

} //: namespace Core

#endif /* EXECUTOR */
