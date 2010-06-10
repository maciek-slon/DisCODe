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
/*
		// Retrieve sources and processors nodes.
		for (xmlNodePtr node = node_settings->children; node != node_settings->next; node = node->next) {
			// Check node.
			if (node->type != XML_ELEMENT_NODE)
				continue;
			if (strcmp((char*) node->name, SOURCES) == 0)
				node_sources = node;
			else if (strcmp((char*) node->name, PROCESSORS) == 0)
				node_processors = node;
		}//: for*/

		// Check whether nodes were found.
		assert(node_sources);
		assert(node_processors);

		LOG(INFO) << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
	}//: else
}

void Configurator::createDefaultConfiguration()
{/*
	// Temporary variables.
	//xmlNodePtr root, node_sources, node_processors;

	// Creates a new document representing configuration.
	configuration = xmlNewDoc(XMLCHARCAST"1.0");
	// Add main node with settings.
	node_settings = xmlNewNode(NULL, XMLCHARCAST "Settings");
	xmlDocSetRootElement(configuration, node_settings);

	// Creates a DTD declaration. Isn't mandatory.
	//    dtd = xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");

	// Add root node new properties.
	xmlNewProp(node_settings, XMLCHARCAST "port", XMLCHARCAST "4000");
	xmlNewProp(node_settings, XMLCHARCAST "gui", XMLCHARCAST "show");
	xmlNewProp(node_settings, XMLCHARCAST "images", XMLCHARCAST "show");

	// Add sources node.
	node_sources = xmlNewChild(node_settings, NULL, XMLCHARCAST SOURCES, NULL);
	xmlNewProp(node_sources, XMLCHARCAST "default", XMLCHARCAST "");

	// Add processors node.
	node_processors = xmlNewChild(node_settings, NULL, XMLCHARCAST PROCESSORS, NULL);
	xmlNewProp(node_processors, XMLCHARCAST "default", XMLCHARCAST "");

	LOG(INFO) << "Configuration: Default configuration created.\n";*/
}

void Configurator::saveConfiguration() {
	// Save current configuration to remembered filename.
	//xmlSaveFormatFileEnc(configuration_filename.c_str(), configuration, "UTF-8", 1);
	write_xml(configuration_filename, configuration);
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
	/*if (xmlChildElementCount(tmp_root)) {
		cout<<"szukam";
		for (xmlNodePtr node = tmp_root->children; node && node != tmp_root->next; node = node->next) {
			// Check node.
			assert(node);
			if (node->type != XML_ELEMENT_NODE)
				continue;
			cout<<"("<<node->name<<") ";
			// Return node if found.
			if (strcmp((char*) node->name, node_name_) == 0)
			{
				cout<<"MAM!\n";
				return node;
			}
		}//: for
	}//: if
	cout<<"nie mam!\n";*/

	// Otherwise - create new child node.
	return &(tmp_root->put_child(node_name, ptree()));
}


}//: namespace Core
