/*!
 * \file Configurator.cpp
 * \brief File containing definitions of Configurator class methods.
 *
 * \author tkornuta
 * \date Apr 8, 2010
 */

#include "Configurator.hpp"
#include "DisCODeException.hpp"
#include "Logger.hpp"

#include "Component.hpp"
#include "Event.hpp"
#include "EventHandler.hpp"
#include "DataStreamInterface.hpp"

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

Task Configurator::loadConfiguration(std::string filename_, const std::vector<std::pair<std::string, std::string> > & overrides)
{
	// Set filename pointer to given one.
	configuration_filename = filename_;

	ptree * tmp_node;

	// Check whether config file exists.
	if (!filesystem::exists(configuration_filename)) {
		throw Common::DisCODeException(std::string("Configuration: File '") + configuration_filename + "' doesn't exist.\n");
	}
	else {
		Task task;

		// Load and parse configuration from file.
		try {
			read_xml(configuration_filename, configuration);
		}
		catch(const xml_parser_error& ex) {
			LOG(LERROR) << "Configuration: Couldn't parse '" << configuration_filename << "' file.";
			LOG(LERROR) << ex.what();
			throw Common::DisCODeException(std::string("Configuration: Couldn't parse '") + configuration_filename + "' file.\n");
		}

		// Take overrides into account
		for (size_t i = 0; i < overrides.size(); ++i) {
			std::cout << overrides[i].first << " set to " << overrides[i].second << std::endl;
			configuration.put(std::string("Task.")+overrides[i].first, overrides[i].second);
		}

		try {
			tmp_node = &(configuration.get_child("Task.Executors"));
			loadExecutors(tmp_node, task);
		}
		catch(ptree_bad_path&) {
			LOG(LERROR) << "No Executors branch in configuration file!\n";
		}

		try {
			tmp_node = &(configuration.get_child("Task.Components"));
			loadComponents(tmp_node, task);
		}
		catch(const ptree_bad_path& ex) {
			LOG(LERROR) << "No Components branch in configuration file!\n";
		}


		try {
			tmp_node = &(configuration.get_child("Task.Events"));
			loadEvents(tmp_node);
		}
		catch(ptree_bad_path&) {
			LOG(LFATAL) << "No Events branch in configuration file!\n";
		}

		try {
			tmp_node = &(configuration.get_child("Task.DataStreams"));
			loadConnections(tmp_node);
		}
		catch(ptree_bad_path&) {
			LOG(LFATAL) << "No DataStreams branch in configuration file!\n";
		}


		LOG(LINFO) << "Configuration: File \'" << configuration_filename << "\' loaded.\n";
		return task;
	}//: else
}

void Configurator::loadExecutors(const ptree * node, Task & task) {
	LOG(LINFO) << "Creating execution threads\n";

	Executor * ex;

	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		ex = executorManager->createExecutor(nd.first, tmp.get("<xmlattr>.type", "UNKNOWN"));
		ex->load(tmp);

		task+=ex;
	}
}

void Configurator::loadComponents(const ptree * node, Task & task) {
	LOG(LINFO) << "Loading required components\n";

	Base::Component * kern;
	Executor * ex;
	std::string name;
	std::string type;
	std::string thread;
	std::string group;
	std::string include;
	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		name = nd.first;

		// ignore coments in tast file
		if (name == "<xmlcomment>") continue;

		type = tmp.get("<xmlattr>.type", "UNKNOWN");
		thread = tmp.get("<xmlattr>.thread", "UNKNOWN");
		group = tmp.get("<xmlattr>.group", "DEFAULT");
		include = tmp.get("<xmlattr>.include", "");

		LOG(LTRACE) << "Component to be created: " << name << " of type " << type << " in thread " << thread << ", subtask " << group << "\n";

		kern = componentManager->createComponent(name, type);

		if (include != "") {
			try {
				read_xml(include, tmp);
			}
			catch(const xml_parser_error& ex) {
				LOG(LERROR) << "Configuration: Couldn't parse include file '" << include << "' for component " << name << ".\n";
				LOG(LERROR) << ex.what();
				throw Common::DisCODeException(std::string("Configuration: Couldn't parse '") + include + "' file.\n");
			}
		}

		if (kern->getProperties()) {
			try {
				kern->getProperties()->load(tmp);
			}
			catch(const ptree_bad_path& ex) {
				LOG(LERROR) << name << ": " << ex.what();
				LOG(LNOTICE) << "Set this property in config file!";

				throw Common::DisCODeException(name + ": failed to load component");
			}
			catch(const ptree_bad_data& ex) {
				LOG(LERROR) << name << ": " << ex.what();
				LOG(LNOTICE) << "Check properties in configuration file!";

				throw Common::DisCODeException(name + ": failed to load component");
			}
		}

		kern->initialize();

		ex = executorManager->getExecutor(thread);
		ex->addComponent(name, kern);

		LOG(LTRACE) << "Adding component " << name << " to subtask " << group << "\n";

		task[group] += kern;

		component_executor[name] = thread;
	}
}

void Configurator::loadEvents(const ptree * node) {
	LOG(LINFO) << "Connecting events\n";
	std::string src, dst, name, caller, receiver, type;
	Base::Component * src_k, * dst_k;
	Base::EventHandlerInterface * h;
	Base::Event * e;
	BOOST_FOREACH( TreeNode nd, *node ) {
		ptree tmp = nd.second;
		name = nd.first;
		if (name == "<xmlcomment>") continue;

		src = tmp.get("<xmlattr>.source", "");
		if (src == "") {
			LOG(LERROR) << "No event source specified...\n";
			continue;
		}

		dst = tmp.get("<xmlattr>.destination", "");
		if (dst == "") {
			LOG(LERROR) << "No event destination specified...\n";
			continue;
		}

		type=tmp.get("type", "");

		caller = src.substr(0, src.find_first_of("."));
		src = src.substr(src.find_first_of(".")+1);

		receiver = dst.substr(0, dst.find_first_of("."));
		dst = dst.substr(dst.find_first_of(".")+1);

		src_k = componentManager->getComponent(caller);
		dst_k = componentManager->getComponent(receiver);

		h = dst_k->getHandler(dst);
		if (!h) {
			LOG(LERROR) << "Component " << receiver << " has no event handler named '" << dst << "'!\n";
			continue;
		}

		e = src_k->getEvent(src);
		if (!e) {
			LOG(LERROR) << "Component " << caller << " has no event named '" << src << "'!\n";
			continue;
		}

		// asynchronous connection
		if ( (component_executor[caller] != component_executor[receiver]) || (type=="async")) {
			Executor * ex = executorManager->getExecutor(component_executor[receiver]);
			h = ex->scheduleHandler(h);
			e->addAsyncHandler(h);
		} else {
			e->addHandler(h);
		}

		LOG(LINFO) << name << ": src=" << src << ", dst=" << dst << "\n";
	}
}

void Configurator::loadConnections(const ptree * node) {
	LOG(LINFO) << "Connecting data streams\n";
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
			if (ds_name == "<xmlcomment>") continue;

			ptree ds_tmp = ds_nd.second;
			type = ds_tmp.get("<xmlattr>.type", "out");
			con_name = ds_tmp.get("<xmlattr>.group", "DefaultGroup");

			con = connectionManager->get(con_name);

			ds = kern->getStream(ds_name);
			if (!ds) {
				LOG(LERROR) << "Component " << name << " has no data stream named '" << ds_name << "'!\n";
			}

			LOG(LINFO) << name << ": str=" << ds_name << " [" << type << "] in " << con_name;

			if (type == "out") {
				ds->setConnection(con);
			} else
			if (type == "in") {
				con->addListener(ds);
			} else {
				LOG(LERROR) << "Unknown data stream type: " << type << "\n";
				continue;
			}
		}
	}
}



}//: namespace Core
