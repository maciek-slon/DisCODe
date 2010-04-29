/*!
 * \file Kernel.hpp
 * \brief Abstract interface for all kernels.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef KERNEL_HPP_
#define KERNEL_HPP_

#include <iostream>
#include <map>
#include <string>

#include <boost/foreach.hpp>

#include "Event.hpp"
#include "EventHandler.hpp"

namespace Base {

/*!
 * \class Kernel
 * \brief Abstract interface class for all modules - data sources, processors etc.
 * \author mstefanc
 * \date 2010-04-29
 */
class Kernel
{
public:
	/*!
	 * Base constructor
	 */
	Kernel()
	{
		std::cout << "Abstract Kernel: Hello\n";
	}

	/*!
	 * Virtual destructor - empty.
	 */
	virtual ~Kernel()
	{
		// delete all events and handlers
		/*BOOST_FOREACH(std::pair<std::string,Event *> event, events) {
			delete event.second;
		}

		BOOST_FOREACH(std::pair<std::string,EventHandler *> handler, handlers) {
			delete handler.second;
		}*/

		std::cout << "Abstract Kernel: Bye \n";
	}

	/*!
	 * Initialize kernel. For example for sources it would be opening streams or devices.
	 */
	virtual void initialize() = 0;

	/*!
	 * Finish kernel work. Here all resources should be released.
	 */
	virtual void finish() = 0;

	/*!
	 * Single work step. For example sources would retrieve single frame,
	 * processors would process one bunch of data.
	 * Method called by kernels owner.
	 * \return execution time
	 */
	virtual int step() = 0;

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
	 * Returns event handler with specified name if registered or NULL.
	 * \param name event handler name
	 * \returns pointer to event handler with specified name or NULL if no such event is registered.
	 */
	EventHandler * getHandler(const std::string& name) {
		if (handlers.count(name) > 0) {
			return handlers[name];
		} else {
			return NULL;
		}
	}

protected:
	/*!
	 * Register new event under specified name.
	 * \param name event name
	 * \returns pointer to newly created event.
	 */
	Event * registerEvent(const std::string& name) {
		// TODO: check, if event already exists
		Event * event = new Event();
		events[name] = event;
		return event;
	}

	/*!
	 * Register new event handler under specified name.
	 * \param name event handler name
	 * \returns pointer to newly created handler.
	 */
	EventHandler * registerHandler(const std::string& name) {
		// TODO: check, if handler already exists
		EventHandler * handler = new EventHandler();
		handlers[name] = handler;
		return handler;
	}

private:
	/// all registered events
	std::map<std::string, Event *> events;

	/// all registered event handlers
	std::map<std::string, EventHandler *> handlers;

};

}//: namespace Base


#endif /* KERNEL_HPP_ */
