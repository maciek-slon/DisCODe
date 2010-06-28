/*!
 * \file main.cpp
 * \brief Main body responsible for menu showing
 * and image processing.
 * \author tkornuta
 * \date 11.09.2007
 */

#include <cstring>
#include <iostream>

#include "FraDIAException.hpp"
#include "ConnectionManager.hpp"
#include "KernelManager.hpp"
#include "ExecutorManager.hpp"
#include "KernelFactory.hpp"
#include "Configurator.hpp"
#include "Executor.hpp"
#include "Logger.hpp"

#include <boost/program_options.hpp>

using namespace std;
using namespace Common;
using namespace Core;

namespace po = boost::program_options;

/*!
 * Main body - creates two threads - one for window and and one
 * for images acquisition/processing.
 */
int main(int argc, char* argv[])
{
	// FraDIA config filename.
	std::string config_name;

	// =========================================================================
	// === Program command-line options
	// =========================================================================

	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("config,C", po::value<std::string>(&config_name)->default_value("config.xml"), "choose config file")
		("create-config,D", po::value<std::string>(&config_name)->default_value("config.xml"), "create default configuration file")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}

	config_name = vm["config"].as<std::string>();




	Configurator configurator;
	KernelManager km;
	ExecutorManager em;

	configurator.setExecutorManager(&em);
	configurator.setKernelManager(&km);

	try {
		km.initializeKernelsList();

		configurator.loadConfiguration(config_name);

		// Test code.

		Core::Executor * ex1;

		Base::Kernel * src = km.getKernel("Camera");

		Base::Kernel * proc = km.getKernel("Window");

		src->printEvents();
		src->printHandlers();
		src->printStreams();

		proc->printEvents();
		proc->printHandlers();
		proc->printStreams();

		// connect src -> newImage event to proc -> onNewImage handler
		Base::EventHandlerInterface * h = proc->getHandler("onNewImage");
		//src->getEvent("newImage")->addHandler(ex2.scheduleHandler(h));
		src->getEvent("newImage")->addHandler(h);

		// connect src -> out_delay data stream to proc -> in_delay data stream
		Base::Connection * con_1 = CONNECTION_MANAGER.get("con_1");
		con_1->addListener(proc->getStream("in_img"));
		if (src->getStream("out_img")) {
			src->getStream("out_img")->setConnection(con_1);
		} else {
			cout << "Stream find error!\n";
		}

		ex1 = em.getExecutor("Thread1");

		// start both threads
		ex1->start();

		Common::Thread::msleep(5000);

		// stop threads
		ex1->finish();

		// wait for both threads to finish execution
		ex1->wait(1000);

		// End of test code.

		src->finish();
		proc->finish();

		km.deactivateKernelList();

	}//: try
	catch (exception& ex){
		cout << "Fatal error:\n";
		// If required print exception description.
		if (!strcmp(ex.what(), ""))
			LOG(FATAL) << ex.what() << "\n";

		exit(EXIT_FAILURE);
	}
	catch (...) {
		LOG(FATAL) << "Unhandled exception.\n";
		exit(EXIT_FAILURE);
	}//: catch
}
