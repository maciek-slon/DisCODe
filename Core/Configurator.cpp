/*!
 * \file Configurator.cpp
 * \brief File containing definitions of Configurator class methods.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#include "Configurator.hpp"
#include "FraDIAException.hpp"

namespace Core {

/*!
 * Macro used during xml strings casting.
 */
#ifndef XMLCHARCAST
#define XMLCHARCAST (const xmlChar *)
#endif

Configurator::Configurator()
{
	// TODO Auto-generated constructor stub

}

Configurator::~Configurator()
{
	// TODO Auto-generated destructor stub
	std::cout << "Elo koniec\n";
}

void Configurator::loadConfiguration(char* filename_)
{
	// Set filename pointer to given one.
	configuration_filename = filename_;

	// Check whether config file exists.
	struct stat buffer;
	if (stat(configuration_filename, &buffer) < 0) {
		std::cout << "Configuration: File \'" << configuration_filename << "\' doesn\'t exist.\n";
		// Create default configuration.
		createDefaultConfiguration();
		// Save it to file.
		saveConfiguration();
	}//: if stat
	else {
		// Load and parse configuration from file.
		configuration = xmlParseFile(configuration_filename);
		if (configuration == NULL)
			throw Common::FraDIAException(std::string("Configuration: Couldn\'t parse \'") + std::string(configuration_filename)
					+ std::string("\' file.\n"));

		// Get root node.
		node_settings = xmlDocGetRootElement(configuration);
		// Retrieve sources and processors nodes.
		for (xmlNodePtr node = node_settings->children; node != node_settings->next; node = node->next) {
			// Check node.
			if (node->type != XML_ELEMENT_NODE)
				continue;
			if (strcmp((char*) node->name, SOURCES) == 0)
				node_sources = node;
			else if (strcmp((char*) node->name, PROCESSORS) == 0)
				node_processors = node;
		}//: for
		// Check whether nodes were found.
		assert(node_sources);
		assert(node_processors);

		std::cout << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
	}//: else
}

void Configurator::createDefaultConfiguration()
{
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

	std::cout << "Configuration: Default configuration created.\n";
}

void Configurator::saveConfiguration() {
	// Save current configuration to remembered filename.
	xmlSaveFormatFileEnc(configuration_filename, configuration, "UTF-8", 1);
	std::cout << "Configuration: Saved to file "<<configuration_filename<<".\n";
}


xmlNodePtr& Configurator::returnManagerNode(Base::kernelType kernel_type_){
	switch(kernel_type_)
	{
		case Base::KERNEL_SOURCE :
			cout<<"!!KERNEL_SOURCE: !!"<<node_sources->name<<endl;
			return node_sources;
			break;
		case Base::KERNEL_PROCESSOR :
			cout<<"!!KERNEL_PROCESSOR: !!"<<node_processors->name<<endl;
			return node_processors;
			break;
	}//: switch

	throw Common::FraDIAException("Configurator::returnManagerNode(): unknown argument");
}


xmlNodePtr Configurator::returnKernelNode(Base::kernelType kernel_type_, const char* node_name_) {
	cout<<node_name_<<": ";
	// Get "root" node for given type of kernels.
	xmlNodePtr tmp_root;
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
	if (xmlChildElementCount(tmp_root)) {
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
	cout<<"nie mam!\n";
	// Otherwise - create new child node.
	xmlNodePtr newnode = xmlNewChild(tmp_root, NULL, XMLCHARCAST node_name_, NULL);
	return newnode;
}


}//: namespace Core
