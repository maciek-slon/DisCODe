/*!
 * \file EventHandler.hpp
 * \brief Component event handler.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include <iostream>

#include "DisCODeException.hpp"

namespace Base {

/*!
 * \class EventHandlerInterface
 * \brief Interface for EventHandler class.
 *
 * Used for storing pointers to different event handlers, without
 * specifying its template arguments.
 *
 * \author mstefanc
 * \date 2010-04-29
 */
class EventHandlerInterface {
public:
	/*!
	 * Execute event handler function.
	 */
	virtual void execute() = 0;

	/*!
	 * Virtual destructor is needed
	 */
	virtual ~EventHandlerInterface() {
	}
};

/*!
 * \class EventHandler
 * \brief EventHandler is called when associated Event is raised.
 *
 * EventHandler calls it's owners method, which must be of type Method
 * (taking no arguments and returning void). If EventHandler is connected to Event, but
 * has no associated owner and method then exception is thrown when event is raised.
 *
 * \author mstefanc
 * \date 2010-04-29
 */
template <class Class>
class EventHandler : public EventHandlerInterface
{
	/*!
	 * \brief Typedef for method belonging to Class, taking no arguments and returning void.
	 */
	typedef void (Class::*Method)();
public:
	/*!
	 * Base constructor
	 */
	EventHandler() {
		owner = NULL;
	}

	/*!
	 * Setup event handler parameters.
	 * \param _owner pointer to object on which function will be called, most often 'this' should be passed
	 * \param _method pointer to method to be called as event handler function
	 */
	void setup(Class* _owner, Method _method) {
		owner = _owner;
		method = _method;
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~EventHandler()	{
	}

	/*!
	 * Convenience function. Execute event handler function.
	 */
	void operator()() {
		execute();
	}

	/*!
	 * Execute event handler function.
	 * \throws DisCODeException when owner is not associated
	 */
	void execute() {
		if (!owner)
			throw Common::DisCODeException("Unassigned event handler called.");

		if (owner->running())
			(owner->*method)();
	}

protected:
	/// Handlers owner. Method is called using owner as this pointer.
	Class*  owner;

	/// Method called when associated event is raised.
	Method  method;
};

/*!
 * \class EventScheduler
 * \brief EventScheduler is proxy object used to queue events raised between threads.
 *
 * EventScheduler takes EventHandler as an argument and when associated event is raised calls
 * queue method (which must be registered before first event occurs). This queue method takes
 * pointer to EventHandlerInterface as an argument and it can for example be inserting
 * EventHandlers to priority queue or to FIFO buffer.
 *
 * Idea of EventScheduler is to make asynchronous event between threads.
 *
 * \author mstefanc
 * \date 2010-05-03
 */
template <class Class>
class EventScheduler : public EventHandlerInterface
{
	/*!
	 * \brief Typedef for queue method belonging to Class, taking one argument and returning void.
	 */
	typedef void (Class::*Method)(EventHandlerInterface *);
public:
	/*!
	 * Base constructor
	 */
	EventScheduler() {
		owner = NULL;
		handler = NULL;
	}

	/*!
	 * Setup event handler parameters.
	 * \param _owner pointer to object on which function will be called, most often 'this' should be passed
	 * \param _method pointer to method to be called as queue function
	 */
	void setup(Class* _owner, Method _method) {
		owner = _owner;
		method = _method;
	}

	/*!
	 * Register event handler to queue
	 */
	void registerHandler(EventHandlerInterface * h) {
		handler = h;
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~EventScheduler()	{
	}

	/*!
	 * Convenience function. Execute event handler function.
	 */
	void operator()() {
		execute();
	}

	/*!
	 * Execute event handler function.
	 * \throws DisCODeException when owner is not associated
	 */
	void execute() {
		if (!owner)
			throw Common::DisCODeException("Unassigned event handler called.");

		if (!handler)
			throw Common::DisCODeException("EventScheduler called without registered handler");

		(owner->*method)(handler);
	}

protected:
	/// Handlers owner. Method is called using owner as this pointer.
	Class*  owner;

	/// Method called when associated event is raised.
	Method  method;

	/// Associated event handler to be scheduled
	EventHandlerInterface * handler;
};

}//: namespace Base


#endif /* EVENTHANDLER_HPP_ */
