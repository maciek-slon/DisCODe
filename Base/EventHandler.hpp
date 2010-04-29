/*!
 * \file EventHandler.hpp
 * \brief Kernel event handler.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include <iostream>

#include "FraDIAException.hpp"

namespace Base {

/*!
 * \class EventHandlerInterface
 * \brief Interface for EventHandler class.
 * Used for storing pointers to different event handlers, without
 * specifying its template arguments.
 * \author mstefanc
 * \date 2010-04-29
 */
class EventHandlerInterface {
public:
	/*!
	 * Execute event handler function.
	 */
	virtual void execute() = 0;
};

/*!
 * \class EventHandler
 * \brief EventHandler is called when associated Event is raised. EventHandler calls it's owners method.
 * \author mstefanc
 * \date 2010-04-29
 */
template <class Class>
class EventHandler : public EventHandlerInterface
{
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
	 * \param _method pointer to method to be called as event hander function
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
	 */
	void execute() {
		if (!owner)
			throw Common::FraDIAException("Unasigned event handler called.");

		(owner->*method)();
	}

protected:
	Class*  owner;
	Method  method;
};

}//: namespace Base


#endif /* EVENTHANDLER_HPP_ */
