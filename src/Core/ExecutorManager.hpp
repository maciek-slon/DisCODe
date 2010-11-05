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
#include "ContinuousExecutor.hpp"
#include "PassiveExecutor.hpp"
#include "PeriodicExecutor.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

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
		types.push_back("continuous");
		types.push_back("periodic");
		types.push_back("passive");
	}

	~ExecutorManager() {

	}

	void release() {
		BOOST_FOREACH(exp e, executors)
			delete e.second;
	}

	Executor * createExecutor(const std::string & name, const std::string & type) {
		if (executors.count(name) > 0) {
			LOG(LWARNING) << "Executor " << name << " already created. Returning previous one.\n";
			return executors[name];
		}

		Executor * ex;
		if (type == "continuous") {
			ex = new ContinuousExecutor(name);
		} else if (type == "passive") {
			ex = new PassiveExecutor(name);
		} else if (type == "periodic") {
			ex = new PeriodicExecutor(name);
		} else {
			LOG(LERROR) << "Executor type " << type << " not allowed!\n";
			LOG(LNOTICE) << "Check executor type for " << name;

			int id = -1, val = 100000;
			for (size_t i = 0; i < types.size(); ++i) {
				int tmp = Utils::distLevenshtein(type, types[i]);
				if (tmp < val) {
					val = tmp;
					id = i;
				}
			}

			LOG(LNOTICE) << "Did you mean " << types[id] << " type?";
			throw Common::DisCODeException("createExecutor");
		}

		executors[name] = ex;
		LOG(LINFO) << name << " (" << type << ") executor created.\n";

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
