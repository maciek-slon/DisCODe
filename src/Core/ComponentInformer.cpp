#include "ComponentInformer.hpp"

#include "ComponentManager.hpp"
#include "CommandInterpreter.hpp"
#include "Property.hpp"

#include <boost/bind.hpp>

namespace Core {

ComponentInformer::ComponentInformer(ComponentManager & cm) : m_component_manager(cm) {
}

void ComponentInformer::registerHandlers(CommandInterpreter & ci) {
	ci.addHandler("listProperties", boost::bind(&ComponentInformer::listProperties,  this, _1));
	ci.addHandler("getProperty", boost::bind(&ComponentInformer::getProperty,  this, _1));
	ci.addHandler("setProperty", boost::bind(&ComponentInformer::setProperty,  this, _1));
}

std::string ComponentInformer::listProperties(std::vector<std::string> args) {
	if (args.empty()) {
		return "No component name specified.";
	}

	Base::Component * component;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	return component->listProperties();
}

std::string ComponentInformer::getProperty(std::vector<std::string> args) {
	if (args.size() != 2) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	return prop->store();
}

std::string ComponentInformer::setProperty(std::vector<std::string> args) {
	if (args.size() != 3) {
		return "No component name specified.";
	}

	Base::Component * component;
	Base::PropertyInterface * prop;

	try {
		component = m_component_manager.getComponent(args[0]);
	}
	catch(...) {
		return "Component not found";
	}

	prop = component->getProperty(args[1]);
	if (!prop) {
		return "Unknown property";
	}

	prop->retrieve(args[2]);

	return prop->store();
}


}
