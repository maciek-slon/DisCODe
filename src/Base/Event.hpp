/*!
 * \file Event.hpp
 * \brief Component event.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <iostream>
#include <list>

#include "EventHandler.hpp"

#include <boost/foreach.hpp>

namespace Base {

/*!
 * \class Event
 * \brief Event raised by Component. Keeps list of it's handlers and calls them when raised.
 * \author mstefanc
 * \date 2010-04-29
 */
class Event
{
public:
	/*!
	 * Base constructor
	 */
	Event()	{
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~Event() {
	}

	void addHandler(EventHandlerInterface * h) {
		handlers.push_back(h);
	}

	void addAsyncHandler(EventHandlerInterface * h) {
		handlers.push_front(h);
	}

	void raise() {
		BOOST_FOREACH(EventHandlerInterface * h, handlers) {
			h->execute();
		}
	}

	void operator()() {
		raise();
	}

protected:
	std::list<EventHandlerInterface *> handlers;

};

}//: namespace Base


#endif /* EVENT_HPP_ */
