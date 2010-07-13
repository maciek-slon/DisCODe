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

#include "Component.hpp"

#include "ComponentManager.hpp"
#include "ExecutorManager.hpp"
#include "ConnectionManager.hpp"

#include <boost/filesystem.hpp>

#include <boost/property_tree/xml_parser.hpp>

namespace Core {

using namespace boost;

Configurator::Configurator()
{

}

Configurator::~Configurator()
{

}

Task Configurator::loadConfiguration(std::string filename_)
{
	// Set filename pointer to given one.
	configuration_filename = filename_;

	ptree * tmp_node;

	// Check whether config file exists.
	if (!filesystem::exists(configuration_filename)) {
		LOG(FATAL) << "Configuration: File '" << configuration_filename << "' doesn't exist.\n";
		throw("loadConfiguration");
	}
	else {
		Task task;

		// Load and parse configuration from file.
		try {
			read_xml(configuration_filename, configuration);
		}
		catch(xml_parser_error&) {
			LOG(FATAL) << "Configuration: Couldn't parse '" << configuration_filename << "' file.\n";
			throw Common::FraDIAException(std::string("Configuration: Couldn't parse '") + configuration_filename + "' file.\n");
		}

		try {
			tmp_node = &(configuration.get_child("Task.Executors"));
			loadExecutors(tmp_node, task);
		}
		catch(ptree_bad_path&) {
			LOG(FATAL) << "No Executors branch in configuration file!\n";
		}

		try {
			tmp_node = &(configuration.get_child("Task.Components"));
			loadComponents(tmp_node, task);
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

		try {
			tmp_node = &(configuration.get_child("Task.DataStreams"));
			loadConnections(tmp_node);
		}
		catch(ptree_bad_path&) {
			LOG(FATAL) << "No DataStreams branch in configuration file!\n";
		}


		LOG(INFO) << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
		return task;
	}//: else
}

void Configurator::loadExecutors(const ptree * node, Task & task) {
	LOG(INFO) << "Creating execution threads\n";

	Executor * ex;

	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		ex = executorManager->createExecutor(nd.first, tmp.get("<xmlattr>.type", "UNKNOWN"));
		ex->load(tmp);

		task+=ex;
	}
}

void Configurator::loadComponents(const ptree * node, Task & task) {
	LOG(INFO) << "Loading required components\n";

	Base::Component * kern;
	Executor * ex;
	std::string name;
	std::string type;
	std::string thread;
	std::string group;
	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		name = nd.first;

		// ignore coments in tast file
		if (name == "<xmlcomment>") continue;

		type = tmp.get("<xmlattr>.type", "UNKNOWN");
		thread = tmp.get("<xmlattr>.thread", "UNKNOWN");
		group = tmp.get("<xmlattr>.group", "DEFAULT");

		LOG(TRACE) << "Component to be created: " << name << " of type " << type << " in thread " << thread << ", subtask " << group << "\n";

		kern = componentManager->createComponent(name, type);

		if (kern->getProperties())
			kern->getProperties()->load(tmp);

		kern->initialize();

		ex = executorManager->getExecutor(thread);
		ex->addComponent(name, kern);

		LOG(TRACE) << "Adding component " << name << " to subtask " << group << "\n";

		task[group] += kern;

		component_executor[name] = thread;
	}
}

void Configurator::loadEvents(const ptree * node) {
	LOG(INFO) << "Connecting events\n";
	std::string src, dst, name, caller, receiver;
	Base::Component * src_k, * dst_k;
	Base::EventHandlerInterface * h;
	Base::Event * e;
	BOOST_FOREACH( TreeNode nd, *node ) {
		ptree tmp = nd.second;
		name = nd.first;
		if (name == "<xmlcomment>") continue;

		src = tmp.get("<xmlattr>.source", "");
		if (src == "") {
			LOG(ERROR) << "No event source specified...\n";
			continue;
		}

		dst = tmp.get("<xmlattr>.destination", "");
		if (dst == "") {
			LOG(ERROR) << "No event destination specified...\n";
			continue;
		}

		caller = src.substr(0, src.find_first_of("."));
		src = src.substr(src.find_first_of(".")+1);

		receiver = dst.substr(0, dst.find_first_of("."));
		dst = dst.substr(dst.find_first_of(".")+1);

		src_k = componentManager->getComponent(caller);
		dst_k = componentManager->getComponent(receiver);

		h = dst_k->getHandler(dst);
		if (!h) {
			LOG(ERROR) << "Component " << receiver << " has no event handler named '" << dst << "'!\n";
			continue;
		}

		e = src_k->getEvent(src);
		if (!e) {
			LOG(ERROR) << "Component " << caller << " has no event named '" << src << "'!\n";
			continue;
		}

		// asynchronous connection
		if (component_executor[caller] != component_executor[receiver]) {
			Executor * ex = executorManager->getExecutor(component_executor[receiver]);
			h = ex->scheduleHandler(h);
		}
		e->addHandler(h);

		LOG(INFO) << name << ": src=" << src << ", dst=" << dst << "\n";
	}
}

void Configurator::loadConnections(const ptree * node) {
	LOG(INFO) << "Connecting data streams\n";
	std::string name, ds_name;
	Base::Component * kern;
	std::string type, con_name;
	Base::Connection * con;
	Base::DataStreamInterface * ds;


	BOOST_FOREACH( TreeNode nd, *node ) {
		ptree tmp = nd.second;
		name = nd.first;
		if (name == "<xmlcomment>") continue;

		kern = componentManager->getComponent(name);
		BOOST_FOREACH( TreeNode ds_nd, tmp ) {
			ds_name = ds_nd.first;
			ptree ds_tmp = ds_nd.second;
			type = ds_tmp.get("<xmlattr>.type", "out");
			con_name = ds_tmp.get("<xmlattr>.group", "DefaultGroup");

			con = connectionManager->get(con_name);

			ds = kern->getStream(ds_name);
			if (!ds) {
				LOG(ERROR) << "Component " << name << " has no data stream named '" << ds_name << "'!\n";
			}

			if (type == "out") {
				ds->setConnection(con);
			} else
			if (type == "in") {
				con->addListener(ds);
			} else {
				LOG(ERROR) << "Unknown data stream type: " << type << "\n";
				continue;
			}
		}
	}
}



}//: namespace Core
