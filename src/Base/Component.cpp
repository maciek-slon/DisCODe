/*!
 * \file Component.cpp
 * \brief Abstract interface for all components.
 */

#include "Component.hpp"

#include "EventHandler.hpp"
#include "DataStreamInterface.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "Property.hpp"

#include <boost/foreach.hpp>

namespace Base {


Component::~Component()
{
}

//------------------------------------------------------------------------------------
// State change methods
//------------------------------------------------------------------------------------
bool Component::initialize() {
	if (state == Unready) {
		if (onInit()) {
			state = Ready;
			return true;
		} else {
			return false;
		}
	}

	if (state == Ready) {
		LOG(LWARNING) << name_ << " already initialized.\n";
		return true;
	}

	if (state == Running) {
		LOG(LWARNING) << name_ << " already initialized and running.\n";
		return true;
	}

	return false;
}

bool Component::start() {
	if (state == Ready) {

		if (onStart()) {
			state = Running;
			return true;
		} else {
			return false;
		}
	}

	if (state == Running) {
		LOG(LWARNING) << name_ << " already running.\n";
		return true;
	}

	if (state == Unready) {
		LOG(LWARNING) << name_ << " is not ready to run.\n";
		return false;
	}

	return false;
}

bool Component::stop() {
	if (state == Running) {
		if (onStop()) {
			state = Ready;
			return true;
		} else {
			return false;
		}
	}

	if (state == Ready) {
		LOG(LWARNING) << name_ << " already stopped.\n";
		return true;
	}

	if (state == Unready) {
		LOG(LWARNING) << name_ << " is not initialized.\n";
		return false;
	}

	return false;
}

bool Component::finish() {
	if (state == Ready) {
		if (onFinish()) {
			state = Unready;
			return true;
		} else {
			return false;
		}
	}

	if (state == Unready) {
		LOG(LWARNING) << name_ << " is already finished.\n";
		return true;
	}

	if (state == Running) {
		LOG(LWARNING) << name_ << " still running. Trying to stop...\n";
		if (stop())
			LOG(LWARNING) << name_ << " stopped. Finishing...\n";
		else
			LOG(LWARNING) << name_ << " didn't stop. Finishing anyway...\n";

		onFinish();

		return false;
	}

	return false;
}

double Component::step() {
	Common::Timer timer;
	if (state == Running) {
		timer.restart();

		typedef std::pair<std::string, std::vector<DataStreamInterface *> > HandlerTriggers;

		BOOST_FOREACH(HandlerTriggers ht, sorted_triggers) {
			bool allready = true;
			CLOG(LDEBUG) << name() << "::" << ht.first;
			BOOST_FOREACH(DataStreamInterface * ds, ht.second) {
				CLOG(LDEBUG) << ds->name() << " is " << (ds->fresh()?"fresh":"old");
				if (!ds->fresh()) {
					allready = false;
					break;
				}
			}
			if (allready) {
				CLOG(LDEBUG) << "All triggers ready for " << ht.first << ". Executing...";
				handlers[ht.first]->execute();
				CLOG(LDEBUG) << ht.first << " execution done.";
			}
		}
	} else {
		CLOG(LWARNING) << name_ << " is not running. Step can't be done.\n";
		return 0;
	}

	return 0;
}

EventHandlerInterface* Component::getReadyHandler() {
	typedef std::pair<std::string, std::vector<DataStreamInterface *> > HandlerTriggers;

	BOOST_FOREACH(HandlerTriggers ht, triggers) {
		bool allready = true;
		CLOG(LDEBUG) << name() << "::" << ht.first;
		BOOST_FOREACH(DataStreamInterface * ds, ht.second) {
			CLOG(LDEBUG) << ds->name() << " is " << (ds->fresh()?"fresh":"old");
			if (!ds->fresh()) {
				allready = false;
				break;
			}
		}
		if (allready) return handlers[ht.first];
	}
	return NULL;
}

//------------------------------------------------------------------------------------
// State check methods
//------------------------------------------------------------------------------------

bool Component::running() const {
	return state == Running;
}

bool Component::initialized() const {
	return state == Ready;
}

//------------------------------------------------------------------------------------
// Event handler related methods
//------------------------------------------------------------------------------------

void Component::printHandlers() {
	LOG(LINFO) << "Registered handlers:\n";
	BOOST_FOREACH(HandlerPair handler, handlers) {
		LOG(LINFO) << "\t" << handler.first << "\n";
	}
}

std::string Component::listHandlers() {
	std::string ret;
	BOOST_FOREACH(HandlerPair handler, handlers) {
		ret += handler.first + "\n";
	}
	return ret;
}

EventHandlerInterface * Component::getHandler(const std::string& name) {
	if (handlers.count(name) > 0) {
		return handlers[name];
	} else {
		return NULL;
	}
}

EventHandlerInterface * Component::registerHandler(const std::string& name, EventHandlerInterface * handler) {
	/// \todo check, if handler already exists
	handlers[name] = handler;
	return handler;
}

void Component::addDependency(const std::string & name, DataStreamInterface* stream) {
	if (!stream) {
		triggers[name].clear();
	} else
	if (stream->type() == DataStreamInterface::dsIn)
		triggers[name].push_back(stream);
	else
		CLOG(LWARNING) << "Handlers can only depend on input streams.";
}

void Component::sortHandlers() {
	typedef std::pair<std::string, std::vector<DataStreamInterface *> > HandlerTriggers;

	sorted_triggers.clear();
	BOOST_FOREACH(HandlerTriggers ht, triggers) {
		std::size_t i = 0;
		for (i = 0; i < sorted_triggers.size(); ++i) {
			if (sorted_triggers[i].second.size() < ht.second.size()) break;
		}
		sorted_triggers.insert(sorted_triggers.begin() + i, ht);
	}
}

//------------------------------------------------------------------------------------
// Data stream related methods
//------------------------------------------------------------------------------------

void Component::printStreams() {
	LOG(LINFO) << "Registered data streams:\n";
	BOOST_FOREACH(StreamPair stream, streams) {
		LOG(LINFO) << "\t" << stream.first << "\n";
	}
}

DataStreamInterface * Component::getStream(const std::string& name) {
	if (streams.count(name) > 0) {
		return streams[name];
	} else {
		return NULL;
	}
}

DataStreamInterface * Component::registerStream(const std::string& name, DataStreamInterface * stream) {
	/// \todo check, if handler already exists
	streams[name] = stream;
	stream->setName(name);
	return stream;
}

//------------------------------------------------------------------------------------
// Property related methods
//------------------------------------------------------------------------------------

void Component::printProperties() {
	if (properties.empty())
		return;

	LOG(LINFO) << "Registered properties:";
	BOOST_FOREACH(PropertyPair prop, properties) {
		LOG(LINFO) << "\t" << prop.first;
	}
}

std::string Component::listProperties() {
	std::string ret;
	BOOST_FOREACH(PropertyPair prop, properties) {
		ret += prop.first + "\n";
	}
	return ret;
}

std::vector<std::string> Component::getAllProperties() {
	std::vector<std::string> ret;
	BOOST_FOREACH(PropertyPair prop, properties) {
		ret.push_back( prop.first );
	}
	return ret;
}

PropertyInterface * Component::getProperty(const std::string& name) {
	if (properties.count(name) > 0) {
		return properties[name];
	} else {
		return NULL;
	}
}

PropertyInterface * Component::registerProperty(const std::string& name, PropertyInterface * prop) {
	/// \todo check, if handler already exists
	properties[name] = prop;
	return prop;
}

PropertyInterface * Component::registerProperty(PropertyInterface & prop) {
	/// \todo check, if handler already exists
	properties[prop.name()] = &prop;
	return &prop;
}


}//: namespace Base

