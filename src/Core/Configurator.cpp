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
#include "EventHandler.hpp"
#include "Property.hpp"
#include "DataStreamInterface.hpp"

#include "ComponentManager.hpp"
#include "ExecutorManager.hpp"
#include "ConnectionManager.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <cstdlib>


namespace Core {

using namespace boost;

Configurator::~Configurator()
{

}

void Configurator::loadConfiguration(const ptree * /* config */) {
	// get enviroment variable containing DCL directory
	char * env_dcldir_tmp = std::getenv("DISCODE_DCL_DIR");
	std::string env_dcldir;
	if (env_dcldir_tmp)
		env_dcldir = env_dcldir_tmp;

	LOG(LINFO) << "ENV[DISCODE_DCL_DIR]='" << env_dcldir << "'";
	boost::split(dcl_locations, env_dcldir, boost::is_any_of(":"));
}

typedef std::pair<std::string, std::string> pss;

std::string & Configurator::substitute(std::string & text, const std::vector<std::pair<std::string, std::string> > & dict) {
	BOOST_FOREACH(pss de, dict) {
		boost::replace_all(text, de.first, de.second);
	}

	return text;
}

void Configurator::expandMacros(ptree & pt, const std::vector<std::pair<std::string, std::string> > & dict) {
	BOOST_FOREACH(ptree::value_type & p, pt) {
		std::string val = pt.get(p.first, "");
		std::string oval = val;

		val = p.second.data();
		oval = val;

		if (val != "") {
			substitute(val, dict);
			//pt.put(p.first, val);
			p.second.put("", val);
			if (val != oval) {
				LOG(LINFO) << "Configurator: " << p.first << " value substituted\n"
							  "\tfrom '" << oval << "' to '" << val << "'";
			}
		}

		expandMacros(p.second, dict);
	}
}

Task Configurator::loadTask(std::string filename_, const std::vector<std::pair<std::string, std::string> > & overrides)
{
	ptree * tmp_node;

	std::vector<std::string> task_parts;
	boost::split(task_parts, filename_, boost::is_any_of(":"));
	if (task_parts.size() == 2) {
		// retrieve task filename from dcl
		std::string dcldir = Utils::findSubdir(task_parts[0], dcl_locations, true);
		if (dcldir == "") dcldir = Utils::findSubdir(std::string("DCL_") + task_parts[0], dcl_locations, true);
		if (dcldir != "") {
			configuration_filename = dcldir + "/tasks/" + task_parts[1] + ".xml";
		} else {
			throw Common::DisCODeException(std::string("Configuration: DCL '") + task_parts[0] + "' doesn't exist.\n");
		}
	} else {
		// Set filename pointer to given one.
		configuration_filename = filename_;
	}

	// Check whether config file exists.
	if (!filesystem::exists(configuration_filename)) {
		throw Common::DisCODeException(std::string("Configuration: File '") + configuration_filename + "' doesn't exist.\n");
	}
	else {
		Task task;
		std::vector<std::pair<std::string, std::string> > dict;

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
		for (std::size_t i = 0; i < overrides.size(); ++i) {
			std::cout << overrides[i].first << " set to " << overrides[i].second << std::endl;
			configuration.put(std::string("Task.")+overrides[i].first, overrides[i].second);
		}

		std::string task_path = boost::filesystem::path(configuration_filename).branch_path().string();
		dict.push_back(std::make_pair("%[TASK_LOCATION]%", task_path));

		// expand macros used in config file
		expandMacros(configuration, dict);

		try {
			tmp_node = &(configuration.get_child("Task.Subtasks"));
			loadSubtasks(tmp_node, task);
		}
		catch(ptree_bad_path&) {
			LOG(LERROR) << "No Subtasks branch in configuration file!\n";
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

void Configurator::loadSubtasks(const ptree * node, Task & task) {
	LOG(LDEBUG) << "Loading subtasks";

	std::string name;
	std::string key;
	Subtask * subtask;

	BOOST_FOREACH(TreeNode nd, *node) {
		ptree tmp = nd.second;
		key = nd.first;

		// ignore comments in task file
		if (key == "<xmlcomment>" || key == "<xmlattr>") {
			continue;
		} else
		if (key != "Subtask") {
			LOG(LWARNING) << "Skipping unknown entry: " << key;
			continue;
		}

		name = tmp.get("<xmlattr>.name", "");

		subtask = &task[name];
		LOG(LDEBUG) << "Created subtask " << name;

		loadExecutors(&tmp, *subtask);
	}
}

void Configurator::loadExecutors(const ptree * node, Subtask & subtask) {
	LOG(LDEBUG) << "Creating execution threads";

	Executor * ex;
	std::string name;
	std::string key;
	std::string type;
	float period;

	BOOST_FOREACH( TreeNode nd, *node ) {
		ptree tmp = nd.second;
		key = nd.first;

		// ignore comments in task file
		if (key == "<xmlcomment>" || key == "<xmlattr>") {
			continue;
		} else
		if (key != "Executor") {
			LOG(LWARNING) << "Skipping unknown entry: " << key;
			continue;
		}

		name = tmp.get("<xmlattr>.name", "");
		type = tmp.get("<xmlattr>.type", "");
		period = tmp.get("<xmlattr>.period", 0.0f);

		ex = executorManager->createExecutor(name);
		ex->setPeriod(period);

		subtask+=ex;

		loadComponents(&tmp, *ex);

		ex->start();
	}
}

void Configurator::loadComponents(const ptree * node, Executor & executor) {
	LOG(LDEBUG) << "Loading required components";

	std::string name;
	std::string type;
	std::string key;
	Base::Component * cmp;
	int prio;
	int bump;

	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		key = nd.first;

		// ignore comments in task file
		if (key == "<xmlcomment>" || key == "<xmlattr>") {
			continue;
		} else
		if (key != "Component") {
			LOG(LWARNING) << "Skipping unknown entry: " << key;
			continue;
		}

		name = tmp.get("<xmlattr>.name", "");
		type = tmp.get("<xmlattr>.type", "");
		prio = tmp.get("<xmlattr>.priority", 0);
		bump = tmp.get("<xmlattr>.bump", 0);

		// split component type into dcl and type
		std::vector<std::string> dcl_comp;
		boost::split(dcl_comp, type, boost::is_any_of(":"));

		// check, if type is properly formed
		if (dcl_comp.size() != 2) {
			LOG(LERROR) << "Bad component type definition: '" << type << "'";
			LOG(LNOTICE) << "Should be in form DCL:COMPONENT";
			throw Common::DisCODeException("Task loading error.");
		} else {
			LOG(LDEBUG) << "\t\t" << name << " [" << dcl_comp[1] << " from " << dcl_comp[0] << "]";
		}

		// try to create requested component
		cmp = componentManager->createComponent(name, dcl_comp[0], dcl_comp[1]);
		cmp->setBump(bump);

		// iterate through properties defined in xml, check if component has them
		// and set them if property is persistent
		loadProperties(&tmp, *cmp);

		cmp->prepareInterface();
		cmp->sortHandlers();

		executor.addComponent(name, cmp, prio);

		component_executor[name] = executor.name();
	}
}

void Configurator::loadProperties(const ptree * node, Base::Component & component) {
	LOG(LDEBUG) << "Loading properties";

	std::string name;
	std::string value;
	std::string key;

	Base::PropertyInterface * prop;

	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		key = nd.first;

		// ignore comments in task file
		if (key == "<xmlcomment>" || key == "<xmlattr>") {
			continue;
		} else
		if (key != "param") {
			LOG(LWARNING) << "Skipping unknown entry: " << key;
			continue;
		}

		name = tmp.get("<xmlattr>.name", "");
		value = tmp.data();

		LOG(LDEBUG) << "Property: " << name << "=" << value;

		prop = component.getProperty(name);
		if (!prop) {
			LOG(LWARNING) << "Component " << component.name() << " has no property named " << name << ", but it is defined in task file.";
			continue;
		}

		if (!prop->isPersistent()) {
			LOG(LWARNING) << "Property " << name << "in " << component.name() << " is not persistent, but is set in task file.";
			continue;
		}

		prop->retrieve(value);
	}

}

void Configurator::loadConnections(const ptree * node) {
	LOG(LDEBUG) << "Connecting data streams";

	std::string src_name, src_port;
	Base::Component * src_cmp;
	Base::DataStreamInterface * src_ds;

	std::string dst_name, dst_port;
	Base::Component * dst_cmp;
	Base::DataStreamInterface * dst_ds;

	std::string key;

	Base::Connection * con;

	BOOST_FOREACH( TreeNode nd, *node) {
		ptree tmp = nd.second;
		key = nd.first;

		// ignore comments in task file
		if (key == "<xmlcomment>" || key == "<xmlattr>") {
			continue;
		} else
		if (key != "Source") {
			LOG(LWARNING) << "Skipping unknown entry: " << key;
			continue;
		}

		src_port = tmp.get("<xmlattr>.name", "");

		src_name = src_port.substr(0, src_port.find_first_of("."));
		src_port = src_port.substr(src_port.find_first_of(".")+1);
		src_cmp = componentManager->getComponent(src_name);
		src_ds = src_cmp->getStream(src_port);
		if (!src_ds) {
			LOG(LERROR) << "Component " << src_name << " has no data stream named '" << src_port << "'!\n";
			continue;
		}

		if (src_ds->type() == Base::DataStreamInterface::dsIn) {
			LOG(LERROR) << "Component " << src_name << ": trying to set input data stream '" << src_port<< "' as output!";
			continue;
		}

		con = connectionManager->get(src_name + ":" + src_port);
		src_ds->setConnection(con);

		BOOST_FOREACH( TreeNode nd2, tmp) {
			key = nd2.first;

			// ignore comments in task file
			if (key == "<xmlcomment>" || key == "<xmlattr>") {
				continue;
			} else
			if (key != "sink") {
				LOG(LWARNING) << "Skipping unknown entry: " << key;
				continue;
			}

			dst_port = nd2.second.data();
			dst_name = dst_port.substr(0, dst_port.find_first_of("."));
			dst_port = dst_port.substr(dst_port.find_first_of(".")+1);
			dst_cmp = componentManager->getComponent(dst_name);
			dst_ds = dst_cmp->getStream(dst_port);
			if (!dst_ds) {
				LOG(LERROR) << "Component " << dst_name << " has no data stream named '" << dst_port << "'!\n";
				continue;
			}

			if (dst_ds->type() == Base::DataStreamInterface::dsOut) {
				LOG(LERROR) << "Component " << src_name << ": trying to set output data stream '" << dst_port << "' as input!";
				continue;
			}

			con->addListener(dst_ds);

			//LOG(LINFO) << src_name << " : " << src_port << " -> " << dst_name << " : " << dst_port;
		}
	}
}



}//: namespace Core
