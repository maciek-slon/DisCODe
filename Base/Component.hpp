/*!
 * \file Component.hpp
 * \brief Abstract interface for all components.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <map>
#include <string>

#include <boost/foreach.hpp>

#include "Event.hpp"
#include "EventHandler.hpp"
#include "DataStreamInterface.hpp"
#include "Logger.hpp"

namespace Base {

class Props;

/*!
 * \class Component
 * \brief Abstract interface class for all modules - data sources, processors etc.
 *
 * Every component should derive from this class, and override at least three methods:
 * initialize, finish and step.
 *
 * For more information about creating components, see \ref dev_components "developing components"
 * in \ref tutorials section.
 * \author mstefanc
 * \date 2010-04-29
 */
class Component
{
	typedef std::pair<std::string, Event *> EventPair;
	typedef std::pair<std::string, EventHandlerInterface *> HandlerPair;
	typedef std::pair<std::string, DataStreamInterface *> StreamPair;

public:
	/*!
	 * Base constructor
	 */
	Component()
	{

	}

	/*!
	 * Virtual destructor
	 */
	virtual ~Component()
	{
		// delete all events and handlers
		BOOST_FOREACH(EventPair event, events) {
			delete event.second;
		}
	}

	/*!
	 * Initialize component. For example for sources it would be opening streams or devices.
	 */
	virtual bool initialize() = 0;

	/*!
	 * Finish component work. Here all resources should be released.
	 */
	virtual bool finish() = 0;

	/*!
	 * Single work step. For example sources would retrieve single frame,
	 * processors would process one bunch of data.
	 * Method called by components owner.
	 * \return execution time
	 */
	virtual int step() = 0;

	/*!
	 * Print list of all registered events.
	 */
	void printEvents() {
		LOG(INFO) << "Registered events:\n";
		BOOST_FOREACH(EventPair event, events) {
			LOG(INFO) << "\t" << event.first << "\n";
		}
	}

	/*!
	 * Returns event with specified name if registered or NULL.
	 * \param name event name
	 * \returns pointer to event with specified name or NULL if no such event is registered.
	 */
	Event * getEvent(const std::string& name) {
		if (events.count(name) > 0) {
			return events[name];
		} else {
			return NULL;
		}
	}

	/*!
	 * Print list of all registered event handlers.
	 */
	void printHandlers() {
		LOG(INFO) << "Registered handlers:\n";
		BOOST_FOREACH(HandlerPair handler, handlers) {
			LOG(INFO) << "\t" << handler.first << "\n";
		}
	}

	/*!
	 * Returns event handler with specified name if registered or NULL.
	 * \param name event handler name
	 * \returns pointer to event handler with specified name or NULL if no such event is registered.
	 */
	EventHandlerInterface * getHandler(const std::string& name) {
		if (handlers.count(name) > 0) {
			return handlers[name];
		} else {
			return NULL;
		}
	}

	/*!
	 * Print list of all registered data streams.
	 */
	void printStreams() {
		LOG(INFO) << "Registered data streams:\n";
		BOOST_FOREACH(StreamPair stream, streams) {
			LOG(INFO) << "\t" << stream.first << "\n";
		}
	}

	/*!
	 * Returns data stream with specified name if registered or NULL.
	 * \param name data stream name
	 * \returns pointer to data stream with specified name or NULL if no such stream is registered.
	 */
	DataStreamInterface * getStream(const std::string& name) {
		if (streams.count(name) > 0) {
			return streams[name];
		} else {
			return NULL;
		}
	}

	/*!
	 * Return pointer to properties of this object.
	 *
	 * Should be overridden in derived classes containing specific properties.
	 */
	virtual Props * getProperties() {
		// by default return NULL indicating, that given component has no properties
		return NULL;
	}

protected:
	/*!
	 * Register new event under specified name.
	 * \param name event name
	 * \returns pointer to newly created event.
	 */
	Event * registerEvent(const std::string& name) {
		/// \todo check, if event already exists
		Event * event = new Event();
		events[name] = event;
		return event;
	}

	/*!
	 * Register new event handler under specified name.
	 * \param name event handler name
	 * \param handler pointer to proper handler
	 * \returns pointer to handler.
	 */
	EventHandlerInterface * registerHandler(const std::string& name, EventHandlerInterface * handler) {
		/// \todo check, if handler already exists
		handlers[name] = handler;
		return handler;
	}

	/*!
	 * Register new data stream under specified name.
	 * \param name stream name
	 * \param stream pointer to proper stream
	 * \returns pointer to handler.
	 */
	DataStreamInterface * registerStream(const std::string& name, DataStreamInterface * stream) {
		/// \todo check, if handler already exists
		streams[name] = stream;
		return stream;
	}

private:
	/// all registered events
	std::map<std::string, Event *> events;

	/// all registered event handlers
	std::map<std::string, EventHandlerInterface *> handlers;

	/// all registered data streams
	std::map<std::string, DataStreamInterface *> streams;

};

}//: namespace Base


#endif /* COMPONENT_HPP_ */
