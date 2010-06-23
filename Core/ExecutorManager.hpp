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
	std::map<std::string, Executor::ExecOpMode> allowed_types;

public:
	ExecutorManager() {
		allowed_types["passive"] = Executor::ExecPassive;
		allowed_types["continous"] = Executor::ExecContinous;
		allowed_types["periodic"] = Executor::ExecPeriodic;
	}

	Executor * createExecutor(const std::string & name, const std::string & type) {
		if (executors.count(name) > 0) {
			LOG(WARNING) << "Executor " << name << " already created. Returning previous one.\n";
			return executors[name];
		}

		if (allowed_types.count(type) < 1) {
			LOG(ERROR) << "Executor type " << type << " not allowed!\n";
			throw Common::FraDIAException("createExecutor");
		}

		Executor * ex = new Executor;
		ex->setExecutionMode(allowed_types[type]);
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
