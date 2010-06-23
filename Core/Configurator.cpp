/*!
 * \file Configurator.cpp
 * \brief File containing definitions of Configurator class methods.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#include "Configurator.hpp"
#include "FraDIAException.hpp"
#include "Logger.hpp"

#include <boost/filesystem.hpp>

namespace Core {

using namespace boost;

Configurator::Configurator()
{

}

Configurator::~Configurator()
{

}

void Configurator::loadConfiguration(std::string filename_)
{
	// Set filename pointer to given one.
	configuration_filename = filename_;

	ptree * tmp_node;

	// Check whether config file exists.
	if (!filesystem::exists(configuration_filename)) {
		LOG(WARNING) << "Configuration: File \'" << configuration_filename << "\' doesn\'t exist.\n";
		throw("loadConfiguration");
	}
	else {
		// Load and parse configuration from file.
		try {
			read_xml(configuration_filename, configuration);
		}
		catch(xml_parser_error) {
			throw Common::FraDIAException(std::string("Configuration: Couldn\'t parse \'") + configuration_filename + "\' file.\n");
		}

		try {
			tmp_node = &(configuration.get_child("Task.Components"));
		}
		catch(ptree_bad_path) {
			LOG(FATAL) << "No Sources branch in configuration file!\n";
		}

		loadKernels(tmp_node);

		LOG(INFO) << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
	}//: else
}

void Configurator::loadKernels(const ptree * node) {
	BOOST_FOREACH( TreeNode nd, *node) {
		std::cout << nd.first << std::endl;

	}
}

}//: namespace Core
