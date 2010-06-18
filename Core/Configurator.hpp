/*!
 * \file Configurator.hpp
 * \brief File containing the declaration of Configurator class.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#ifndef CONFIGURATOR_HPP_
#define CONFIGURATOR_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <sys/stat.h>
#include <iostream>
#include <string>

#include "Singleton.hpp"
#include "Names_Aux.hpp"
#include "Kernel_Aux.hpp"

namespace Core {

using namespace boost::property_tree;

/*!
 * \class Configurator
 * \brief Class responsible for configuration management.
 * Inherits singleton-behavior from the Singleton class.
 * \date April 8 2010
 * \author tkornuta
 */
class Configurator//: public Base::Singleton <Configurator>
{

	/*!
	 * Singleton class must be a friend, because only it can call protected constructor.
	 */
	//friend class Base::Singleton <Configurator>;
private:

//protected:

	typedef std::pair<std::string, ptree> TreeNode;

	/*!
	 * Stored configuration.
	 */
	ptree configuration;

	/*!
	 * Configuration filename.
	 */
	std::string configuration_filename;

	/*!
	 * Main XmlNode with settings.
	 */
	bool node_settings;

	/*!
	 * XmlNode with sources.
	 */
	ptree * node_sources;

	/*!
	 * XmlNode with processors.
	 */
	ptree * node_processors;

public:
	Configurator();

	virtual ~Configurator();

	/*!
	 * Loads configuration from xml file.
	 */
	void loadConfiguration(std::string filename);

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
	ptree * returnManagerNode(Base::kernelType kernel_type_);

	/*!
	 * Returns existing (or creates new) node for kernel of given type.
	 */
	ptree * returnKernelNode(Base::kernelType kernel_type_, const char* node_name_);

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
