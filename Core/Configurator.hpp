/*!
 * \file Configurator.hpp
 * \brief File containing the declaration of Configurator class.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#ifndef CONFIGURATOR_HPP_
#define CONFIGURATOR_HPP_

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <sys/stat.h>
#include <iostream>

#include "Singleton.hpp"
#include "Names_Aux.hpp"
#include "Kernel_Aux.hpp"

namespace Core {

/*!
 * \class Configurator
 * \brief Class responsible for configuration management.
 * Inherits singleton-behavior from the Singleton class.
 * \date April 8 2010
 * \author tkornuta
 */
class Configurator: public Base::Singleton <Configurator>
{
	/*!
	 * Singleton class must be a friend, because only it can call protected constructor.
	 */
	friend class Base::Singleton <Configurator>;
private:
	/*!
	 * Private constructor.
	 */
	Configurator();

//protected:
	/*!
	 * Stored configuration.
	 */
	xmlDocPtr configuration;

	/*!
	 * Configuration filename.
	 */
	char* configuration_filename;

	/*!
	 * Main XmlNode with settings.
	 */
	xmlNodePtr node_settings;

	/*!
	 * XmlNode with sources.
	 */
	xmlNodePtr node_sources;

	/*!
	 * XmlNode with processors.
	 */
	xmlNodePtr node_processors;

public:
	virtual ~Configurator();

	/*!
	 * Loads configuration from xml file.
	 */
	void loadConfiguration(char* filename_);

	/*!
	 * Creates xml document with default nodes: root (Settings) and children related to KernelManages (Soures, Processors).
	 */
	void createDefaultConfiguration();

	/*!
	 * Saves configuration from xml doc to file.
	 */
	void saveConfiguration();

	/*!
	 * Returns node related to one of the managers.
	 */
	xmlNodePtr& returnManagerNode(Base::kernelType kernel_type_);

	/*!
	 * Returns existing (or creates new) node for kernel of given type.
	 */
	xmlNodePtr returnKernelNode(Base::kernelType kernel_type_, const char* node_name_);

};

}//: namespace Core

/*!
 * \def CONFIGURATOR
 * \brief A macro for shorten the call to retrieve the instance of configurator.
 * \author tkornuta
 * \date Apr 9, 2010
 */
#define CONFIGURATOR Core::Configurator::instance()

#endif /* CONFIGURATOR_HPP_ */
