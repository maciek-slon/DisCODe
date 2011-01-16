#ifndef EXECUTORINFORMER_HPP_
#define EXECUTORINFORMER_HPP_

#include "Informer.hpp"

#include <string>
#include <vector>

namespace Core {

class ExecutorManager;

class ExecutorInformer : public Informer
{
public:
	ExecutorInformer(ExecutorManager & cm);

	void registerHandlers(CommandInterpreter & ci);

	std::string listComponents(std::vector<std::string> args);

private:
	ExecutorManager & m_executor_manager;
};

}

#endif // EXECUTORINFORMER_HPP_
