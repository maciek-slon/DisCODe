/*!
 * \file main.cc
 * \brief Main body responsible for menu showing
 * and image processing.
 * \author tkornuta
 * \date 11.09.2007
 */

/*!
 * \mainpage FraDIA
 * The main goal of the FraDIA (Framework for Digital Image Analysis) was the merging of
 * image processing algorithms of all Robot Control and Recognition Systems Team's
 * diplomants in one tool, which will provide standardized and rapid method for the
 * implementation of diverse image processing algorithms.
 *
 * Current version wrappes the functionality of the OpenCV library
 * (http://sourceforge.net/projects/opencv/), offering the possibility of retrieving
 * images from a range of sources (camera, movie, image), as well as recording movies
 * (avi files) and catching single frames (png files). All images are stored in the
 * IplImage structure, derived from the Intel's IPP.
 *
 * User interface is constructed with the use of the FLTK (http://www.fltk.org/), which facilitates the UI
 * modifications required for every user's task.
 *
 * Framework automates the way of adding new tasks (interfaces and processing algorithms)
 * to the application - user has to implement two basic classes and enclose them
 * in dynamic library (so). At the start application retrieves information from all
 * shared objects found in current directory (.) and fills menu bar with names of
 * tasks, which enables switching between tasks without application's restart.
 *
 * FraDIA can work as a stand-alone application or as one of the processes of the controller based on the
 * MROCC++ framework. In the second case information retrieved from processed images is passed to the other
 * processes of the controller.
 *
 * Currently FraDIA also enables the use of GPGPU computations (basing on the CUDA library),
 * which can drastically reduce the time of images processing (e.g. filtering, color space transformations).
 *
 *
 * \author tkornuta
 * \date 10.08.2008
 */

/*!
 * \namespace Core
 * \brief The Core namespace contains classes and structures essential for application's work, including threads synchronization,
 * tasks management and GUI.
 * \author tkornuta
 */

/*!
 * \namespace Base
 * \brief Contains abstract base classes, e.g. task panel, image processor and drawable object.
 * \author tkornuta
 */

/*!
 * \namespace Common
 * \brief This namespace contains classes usefull in different tasks, like region segmentators, space color transformators, LUT color classifiers, etc.
 * \author tkornuta
 */

/*!
 * \namespace Sources
 * \brief The Sources namespace contains classes and structures responsible for image retrieval from different sources (images, movies, cameras, etc).
 * tasks management and GUI.
 * \author tkornuta
 */

#include <pthread.h>

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>


#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <iostream>


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
		if (ex.what() != "")
			cout << ex.what() << endl;
		// Show usage.
		//cout << "Usage:\n fradia [config]\n"
		//		<< "Options:\n config\t Sets different config XML file from the default config.xml.\n";
		exit(EXIT_FAILURE);
	}//: catch
}
