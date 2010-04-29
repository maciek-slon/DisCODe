/*!
 * \file Event.hpp
 * \brief Kernel event.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <iostream>
#include <list>

#include "EventHandler.hpp"

namespace Base {

/*!
 * \class Event
 * \brief Event raised by Kernel. Keeps list of it's handlers and calls them when raised.
 * \author mstefanc
 * \date 2010-04-29
 */
class Event
{
public:
	/*!
	 * Base constructor
	 */
	Event()
	{
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~Event()
	{
	}

	void addHandler(const EventHandler& h) {
		handlers.push_back(h);
	}

protected:
	std::list<EventHandler> handlers;

};

}//: namespace Base


#endif /* EVENT_HPP_ */
