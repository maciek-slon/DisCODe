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
	node_sources = NULL;
	node_processors = NULL;
}

Configurator::~Configurator()
{
	std::cout << "Elo koniec\n";
}

void Configurator::loadConfiguration(std::string filename_)
{
	// Set filename pointer to given one.
	configuration_filename = filename_;

	// Check whether config file exists.
	if (!filesystem::exists(configuration_filename)) {
		LOG(WARNING) << "Configuration: File \'" << configuration_filename << "\' doesn\'t exist.\n";
		// Create default configuration.
		createDefaultConfiguration();
		// Save it to file.
		saveConfiguration();
	}//: if stat
	else {
		// Load and parse configuration from file.
		try {
			read_xml(configuration_filename, configuration);
		}
		catch(xml_parser_error) {
			throw Common::FraDIAException(std::string("Configuration: Couldn\'t parse \'") + configuration_filename + "\' file.\n");
		}

		try {
			node_sources = & (configuration.get_child("Settings.Sources"));
		}
		catch(ptree_bad_path) {
			LOG(FATAL) << "No Sources branch in configuration file!\n";
		}

		try {
			node_processors = & (configuration.get_child("Settings.Processors"));
		}
		catch(ptree_bad_path) {
			LOG(FATAL) << "No Processors branch in configuration file!\n";
		}

		// Check whether nodes were found.
		assert(node_sources);
		assert(node_processors);

		LOG(INFO) << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
	}//: else
}

void Configurator::createDefaultConfiguration()
{
	configuration.clear();

	configuration.put("Settings.<xmlattr>.port", 4000);
	configuration.put("Settings.<xmlattr>.gui", "show");
	configuration.put("Settings.<xmlattr>.images", "show");

	configuration.put("Settings.Sources.<xmlattr>.default", "");
	configuration.put("Settings.Processors.<xmlattr>.default", "");

	LOG(INFO) << "Configuration: Default configuration created.\n";
}

void Configurator::saveConfiguration() {
	// Save current configuration to remembered filename.
	xml_writer_settings<ptree::key_type::value_type> settings('\t', 1, "utf-8");
	write_xml(configuration_filename, configuration, std::locale(), settings);

	LOG(INFO) << "Configuration: Saved to file " << configuration_filename << ".\n";
}


ptree * Configurator::returnManagerNode(Base::kernelType kernel_type_){
	switch(kernel_type_)
	{
		case Base::KERNEL_SOURCE :
			return node_sources;
		case Base::KERNEL_PROCESSOR :
			return node_processors;
	}//: switch

	throw Common::FraDIAException("Configurator::returnManagerNode(): unknown argument");
}


ptree * Configurator::returnKernelNode(Base::kernelType kernel_type_, const char* node_name_) {
	cout<<node_name_<<": ";
	// Get "root" node for given type of kernels.
	ptree * tmp_root = NULL;
	switch(kernel_type_)
	{
		case Base::KERNEL_SOURCE :
			tmp_root = node_sources;
			cout<<"source: ";
			break;
		case Base::KERNEL_PROCESSOR :
			tmp_root = node_processors;
			cout<<"processor: ";
			break;
		default:
			break;
	}//: switch
	assert(tmp_root);

	// Try to find node related to given kernel.
	try {
		ptree * ret = &(tmp_root->get_child(node_name_));
		LOG(INFO) << "Mam " << node_name_ << "\n";
		return ret;
	}
	catch(ptree_bad_path) {
		// Otherwise - create new child node.
		LOG(INFO) << "Nie mam, tworzę " << node_name_ << "\n";
		return &(tmp_root->put_child(node_name_, ptree()));
	}
}


}//: namespace Core
