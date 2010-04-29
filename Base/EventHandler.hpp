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

namespace Base {

/*!
 * \class EventHandler
 * \brief EventHandler is called when associated Event is raised. EventHandler calls it's owners method.
 * \author mstefanc
 * \date 2010-04-29
 */
class EventHandler
{
public:
	/*!
	 * Base constructor
	 */
	EventHandler()
	{
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~EventHandler()
	{
	}

protected:


};

}//: namespace Base


#endif /* EVENTHANDLER_HPP_ */
