#ifndef COMPONENTINFORMATOR_HPP
#define COMPONENTINFORMATOR_HPP

#include "Informer.hpp"

#include <string>
#include <vector>

namespace Core {

class ComponentManager;

class ComponentInformer : public Informer
{
public:
	ComponentInformer(ComponentManager & cm);

	void registerHandlers(CommandInterpreter & ci);


	std::string listProperties(std::vector<std::string> args);

	std::string getProperty(std::vector<std::string> args);
	std::string setProperty(std::vector<std::string> args);

	std::string getPropertyType(std::vector<std::string> args);

	std::string getPropertyToolTip(std::vector<std::string> args);

	std::string getPropertyConstraints(std::vector<std::string> args);

	std::string listHandlers(std::vector<std::string> args);
	std::string triggerHandler(std::vector<std::string> args);


private:
	ComponentManager & m_component_manager;
};

}

#endif // COMPONENTINFORMATOR_HPP
