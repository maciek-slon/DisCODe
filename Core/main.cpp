/*!
 * \file main.cpp
 * \brief Main body responsible for menu showing and image processing.
 * \author tkornuta
 * \date 11.09.2007
 */

#include <cstring>
#include <iostream>
#include <fstream>
#include <utility>

#include <signal.h>

#include "FraDIAException.hpp"
#include "ConnectionManager.hpp"
#include "ComponentManager.hpp"
#include "ExecutorManager.hpp"
#include "ComponentFactory.hpp"
#include "Configurator.hpp"
#include "Executor.hpp"
#include "Logger.hpp"

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace Common;
using namespace Core;

namespace po = boost::program_options;
namespace pt = boost::property_tree;

volatile bool running = true;
bool unstopable = false;

void terminate (int param) {
	if (unstopable) {
		std::cout << "\rGet lost! WMAHAHA!\n";
		return;
	}

	std::cout << "\rTerminating program...\n";
	running = false;
}

/*!
 * Main body - creates two threads - one for window and and one
 * for images acquisition/processing.
 */
int main(int argc, char* argv[])
{
	// FraDIA config filename.
	std::string config_name;

	// Task config filename.
	std::string task_name;

	// Task settings overrides
	std::vector <std::string> task_overrides;

	// Log level
	int log_lvl = 2;

	void (*prev_fn)(int);

	prev_fn = signal (SIGINT, terminate);
	if (prev_fn==SIG_IGN) signal (SIGINT,SIG_IGN);

	// =========================================================================
	// === Program command-line options
	// =========================================================================

	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("config,C", po::value<std::string>(&config_name)->default_value("config.xml"), "choose config file")
		("create-config,D", "create default configuration file")
		("task,T", po::value<std::string>(&task_name), "choose task file")
		("create-task", "create default task file")
		("log-level,L", po::value<int>(&log_lvl)->default_value(3), "set log severity level")
		("unstopable","MWAHAHAHA!")
		("set,S",po::value< vector<string> >(&task_overrides),"override task settings")
	;

	po::variables_map vm;

	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch (const po::error & u) {
		std::cout << u.what() << "\n";
		return 0;
	}

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}

	// set logger severity level
	LOGGER.setLevel((Utils::Logger::Severity)log_lvl);
	LOGGER.addOutput(new Utils::Logger::ConsoleOutput, (Utils::Logger::Severity)log_lvl);

	if (vm.count("create-task")) {
		cout << "Creating task file " << task_name << "\n";

		std::ofstream cfg(task_name.c_str());

		cfg << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Task>\n"
				"\t<!-- List of components -->\n"
				"\t<Components>\n"
				"\t</Components>\n"
				"\n"
				"\t<!-- Threads of execution -->\n"
				"\t<Executors>\n"
				"\t\t<Thread1 type=\"passive\">\n"
				"\t\t</Thread1>\n"
				"\t</Executors>\n"
				"\n"
				"\t<!-- Event connections -->\n"
				"\t<Events>\n"
				"\t</Events>\n"
				"\n"
				"\t<!-- Data stream assignments -->\n"
				"\t<DataStreams>\n"
				"\t</DataStreams>\n"
				"</Task>\n";

		cfg.close();

		return 0;
	}

	if (vm.count("create-config")) {
		cout << "Creating configuration file " << config_name << "\n";

		std::ofstream cfg(config_name.c_str());

		cfg << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<FraDIA>\n"
				"\t<task>blob.xml</task>"
				"</FraDIA>\n";

		cfg.close();

		return 0;
	}

	if (vm.count("unstopable")) {
		unstopable = true;
	}

	// =========================================================================
	// === FraDIA configuration
	// =========================================================================

	config_name = vm["config"].as<std::string>();

	pt::ptree conf;
	try {
		read_xml(config_name, conf);
	}
	catch(xml_parser_error&) {
		throw Common::FraDIAException(std::string("Configuration: Couldn't parse '") + config_name + "' file.\n");
	}

	if (!vm.count("task")) {
		task_name = conf.get("FraDIA.task","task.xml");
		LOG(INFO) << "Task: " << task_name << " from configuration file\n";
	} else {
		LOG(INFO) << "Task: " << task_name << " from command line\n";
	}

	std::vector<std::pair<std::string, std::string> > overrides;
	for (int i = 0; i < task_overrides.size(); ++i) {
		std::vector<std::string> strs;
		boost::split(strs, task_overrides[i], boost::is_any_of("="));
		if (strs.size() == 1) {
			LOG(WARNING) << strs[0] << "have no assigned value";
		} else {
			overrides.push_back(std::make_pair(strs[0], strs[1]));
		}
	}

	// =========================================================================
	// === Main program part
	// =========================================================================

	Configurator configurator;
	ComponentManager km;
	ExecutorManager em;
	ConnectionManager cm;

	configurator.setExecutorManager(&em);
	configurator.setComponentManager(&km);
	configurator.setConnectionManager(&cm);

	try {
		Task task;

		km.initializeComponentsList();

		task = configurator.loadConfiguration(task_name, overrides);
		if (!task["s1"].start())
			LOG(WARNING) << "Subtask S1 start() returned false\n";

		task.start();

		while(running) {
			Common::Thread::msleep(50);
		}

		//Common::Thread::msleep(5000);

		Common::Thread::msleep(500);

		task.finish();

		// wait for threads to finish execution
		//Common::Thread::msleep(3000);

		// End of test code.



		km.release();
		cm.release();
		em.release();

		km.deactivateComponentList();

	}//: try

	// =========================================================================
	// === Exception handling
	// =========================================================================

	catch (Common::FraDIAException& ex) {
		LOG(FATAL) << ex.what() << "\n";
		ex.printStackTrace();
		exit(EXIT_FAILURE);
	}
	catch (exception& ex) {
		LOG(FATAL) << ex.what() << "\n";
		exit(EXIT_FAILURE);
	}
	catch (const char * ex) {
		LOG(FATAL) << ex << "\n";
		exit(EXIT_FAILURE);
	}
	catch (...) {
		LOG(FATAL) << "Unknown exception.\n";
		exit(EXIT_FAILURE);
	}//: catch
}
