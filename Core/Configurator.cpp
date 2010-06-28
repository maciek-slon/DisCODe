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

#include "Kernel.hpp"

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
		LOG(WARNING) << "Configuration: File '" << configuration_filename << "' doesn't exist.\n";
		throw("loadConfiguration");
	}
	else {
		// Load and parse configuration from file.
		try {
			read_xml(configuration_filename, configuration);
		}
		catch(xml_parser_error&) {
			throw Common::FraDIAException(std::string("Configuration: Couldn't parse '") + configuration_filename + "' file.\n");
		}

		try {
			tmp_node = &(configuration.get_child("Task.Executors"));
			loadExecutors(tmp_node);
		}
		catch(ptree_bad_path&) {
			LOG(FATAL) << "No Executors branch in configuration file!\n";
		}

		try {
			tmp_node = &(configuration.get_child("Task.Components"));
			loadKernels(tmp_node);
		}
		catch(ptree_bad_path&) {
			LOG(FATAL) << "No Components branch in configuration file!\n";
		}

		try {
			tmp_node = &(configuration.get_child("Task.Events"));
			loadEvents(tmp_node);
		}
		catch(ptree_bad_path&) {
			LOG(FATAL) << "No Events branch in configuration file!\n";
		}


		LOG(INFO) << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
	}//: else
}

void Configurator::loadExecutors(const ptree * node) {
	LOG(INFO) << "Creating execution threads\n";

	Executor * ex;

	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		ex = executorManager->createExecutor(nd.first, tmp.get("<xmlattr>.type", "UNKNOWN"));
		ex->load(tmp);
	}
}

void Configurator::loadKernels(const ptree * node) {
	LOG(INFO) << "Loading required components\n";

	Base::Kernel * kern;
	Executor * ex;
	std::string name;
	std::string type;
	std::string thread;
	bool main;
	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		name = nd.first;
		type = tmp.get("<xmlattr>.type", "UNKNOWN");
		thread = tmp.get("<xmlattr>.thread", "UNKNOWN");
		main = tmp.get("<xmlattr>.main", false);

		kern = kernelManager->createKernel(name, type);

		if (kern->getProperties())
			kern->getProperties()->load(tmp);

		ex = executorManager->getExecutor(thread);
		ex->addKernel(kern, main);

		kern->initialize();
	}
}

void Configurator::loadEvents(const ptree * node) {
	LOG(INFO) << "Connecting events\n";
	std::string src, dst, name;
	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		name = nd.first;
		src = tmp.get("<xmlattr>.source", "UNKNOWN");
		dst = tmp.get("<xmlattr>.destination", "UNKNOWN");

		std::cout << name << ": src=" << src << ", dst=" << dst << "\n";
	}
}



}//: namespace Core
