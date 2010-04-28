/*!
 * \file main.cc
 * \brief Main body responsible for menu showing
 * and image processing.
 * \author tkornuta
 * \date 11.09.2007
 */

#include <pthread.h>

#include <sys/time.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>


#include "FraDIAException.hpp"

#include "KernelManager.hpp"
#include "KernelFactory.hpp"
#include "Configurator.hpp"

using namespace std;
using namespace Common;
using namespace Core;


xmlDocPtr configuration;
char* configuration_filename;

/*!
 * Main body - creates two threads - one for window and and one
 * for images acquisition/processing.
 */
int main(int argc_, char** argv_)
{

/*	// Creates a new document representing configuration.
	configuration = xmlNewDoc(XMLCHARCAST "1.0");
	// Add root node.

	xmlNodePtr root = xmlNewNode(NULL, XMLCHARCAST "settings");
    xmlDocSetRootElement(configuration, root);

     // Creates a DTD declaration. Isn't mandatory.
//    dtd = xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");

    // Add root node.
    xmlSetProp(root, XMLCHARCAST "port", XMLCHARCAST "4000");
    xmlSetProp(root, XMLCHARCAST "gui", XMLCHARCAST "show");
    xmlSetProp(root, XMLCHARCAST "images", XMLCHARCAST "show");
    xmlSetProp(root, XMLCHARCAST "default1", XMLCHARCAST "Image");*/
/*    xmlSetProp(root, XMLCHARCAST "default_task", XMLCHARCAST "TK_SimpleExample");
    xmlUnsetProp(root, XMLCHARCAST "default_task");
    xmlNewProp(root, XMLCHARCAST "default_task", XMLCHARCAST "AAA");*/

//    xmlSetProp(root, XMLCHARCAST "default_task", XMLCHARCAST "TK_SimpleExample");
//    xmlSetProp(root, XMLCHARCAST "default_task", XMLCHARCAST "AAA");



/*	Base::myState::instance().loadStateFromNode(root);
	Base::my_struct st = Base::myState::instance().getState();
	cout<<"port="<<st.port<<endl;
	cout<<"default_task="<<st.task<<endl;*/

/*	Base::stringState::instance().setNode(root);
	//root = Base::stringState::instance().getNode();
    //xmlSetProp(root, XMLCHARCAST "default", XMLCHARCAST "TK_SimpleExample2");
	//root = Base::stringState::instance().getNode();

    xmlSaveFormatFileEnc("aaa.xml", configuration, "UTF-8", 1);

	cout<<"default_task="<<Base::stringState::instance().getState()<<endl;*/

//	exit(1);

	try {
		// FraDIA config filename.
		char *config_name;
		// Check whether other file wasn't pointed.
		if (argc_ == 2)
			config_name = argv_[1];
		else
			// Default configuration file.
			config_name = "config.xml";

		CONFIGURATOR.loadConfiguration(config_name);

		SOURCES_MANAGER.initializeKernelsList();
		PROCESSORS_MANAGER.initializeKernelsList();

		CONFIGURATOR.saveConfiguration();

	}//: try
	catch (exception& ex){
		// If required print exception description.
		cout << "Fatal exception:\n";
		if (!strcmp(ex.what(), ""))
			cout << ex.what() << endl;
		// Show usage.
		//cout << "Usage:\n fradia [config]\n"
		//		<< "Options:\n config\t Sets different config XML file from the default config.xml.\n";
		exit(EXIT_FAILURE);
	}//: catch
}
