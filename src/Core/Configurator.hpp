/*!
 * \file Configurator.hpp
 * \brief File containing the declaration of Configurator class.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#ifndef CONFIGURATOR_HPP_
#define CONFIGURATOR_HPP_

#include "Task.hpp"

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <map>

namespace Core {

using namespace boost::property_tree;

class ComponentManager;
class ExecutorManager;
class ConnectionManager;

/*!
 * \class Configurator
 * \brief Class responsible for configuration management.
 * Inherits singleton-behavior from the Singleton class.
 * \date April 8 2010
 * \author tkornuta
 */
class Configurator
{
private:

	typedef std::pair<std::string, ptree> TreeNode;

	/*!
	 * Stored configuration.
	 */
	ptree configuration;

	/*!
	 * Configuration filename.
	 */
	std::string configuration_filename;

	ExecutorManager * executorManager;
	ComponentManager * componentManager;
	ConnectionManager * connectionManager;

	/*!
	 * List containing execution thread name for each component
	 */
	std::map<std::string, std::string> component_executor;

public:
	Configurator();

	virtual ~Configurator();

	/*!
	 * Loads configuration from xml file.
	 */
	Task loadConfiguration(std::string filename, const std::vector<std::pair<std::string, std::string> > & overrides);

	void loadExecutors(const ptree * node, Task & task);
	void loadComponents(const ptree * node, Task & task);
	void loadEvents(const ptree * node);
	void loadConnections(const ptree * node);

	void setExecutorManager(ExecutorManager * em) {
		executorManager = em;
	}

	void setComponentManager(ComponentManager * km) {
		componentManager = km;
	}

	void setConnectionManager(ConnectionManager * cm) {
		connectionManager = cm;
	}
};

}//: namespace Core

#endif /* CONFIGURATOR_HPP_ */
