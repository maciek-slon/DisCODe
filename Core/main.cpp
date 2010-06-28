/*!
 * \file main.cpp
 * \brief Main body responsible for menu showing and image processing.
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
		("create-config,D", "create default configuration file")
	;

	po::variables_map vm;

	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch (po::unknown_option & u) {
		LOG(FATAL) << u.what() << "\n";
		return 0;
	}

	if (vm.count("help")) {
		cout << desc << "\n";
		return 0;
	}

	if (vm.count("create-config")) {
		cout << "Creating config file " << config_name << "\n";
		return 0;
	}

	config_name = vm["config"].as<std::string>();

	// =========================================================================
	// === Main program part
	// =========================================================================

	Configurator configurator;
	KernelManager km;
	ExecutorManager em;
	ConnectionManager cm;

	configurator.setExecutorManager(&em);
	configurator.setKernelManager(&km);
	configurator.setConnectionManager(&cm);

	try {
		km.initializeKernelsList();

		configurator.loadConfiguration(config_name);

		// Test code.

		Core::Executor * ex1;

		Base::Kernel * src = km.getKernel("Camera");
		Base::Kernel * proc = km.getKernel("Window");

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

	// =========================================================================
	// === Exception handling
	// =========================================================================

	catch (exception& ex){
		cout << "Fatal error:\n";
		// If required print exception description.
		if (!strcmp(ex.what(), ""))
			LOG(FATAL) << ex.what() << "\n";

		exit(EXIT_FAILURE);
	}
	catch (const char * ex) {
		LOG(FATAL) << ex << "\n";
		exit(EXIT_FAILURE);
	}
	catch (...) {
		LOG(FATAL) << "Unhandled exception.\n";
		exit(EXIT_FAILURE);
	}//: catch
}
