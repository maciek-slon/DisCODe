#include "ExecutorInformer.hpp"

#include "ExecutorManager.hpp"
#include "CommandInterpreter.hpp"
#include "Executor.hpp"

#include <boost/bind.hpp>

namespace Core {

ExecutorInformer::ExecutorInformer(ExecutorManager & em) : m_executor_manager(em) {
}

void ExecutorInformer::registerHandlers(CommandInterpreter & ci) {
	ci.addHandler("listComponents", boost::bind(&ExecutorInformer::listComponents,  this, _1));
}

std::string ExecutorInformer::listComponents(std::vector<std::string> args) {
	if (args.empty()) {
		return "No executor name specified.";
	}

	Core::Executor * executor;

	try {
		executor = m_executor_manager.getExecutor(args[0]);
	}
	catch(...) {
		return "Executor not found";
	}

	std::string ret;
	std::vector<std::string> tmp = executor->listComponents();
	BOOST_FOREACH(std::string s, tmp) {
		ret += s + "\n";
	}
	return ret;

}



}
