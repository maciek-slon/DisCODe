#ifndef EVENTHANDLER2_HPP
#define EVENTHANDLER2_HPP

#include "EventHandler.hpp"
#include <boost/function.hpp>

namespace Base {

class EventHandler2 : public EventHandlerInterface {
public:
	/*!
	 * Setup event handler parameters.
	 * \param _owner pointer to object on which function will be called, most often 'this' should be passed
	 * \param _method pointer to method to be called as event handler function
	 */
	void setup(boost::function<void()> f) {
		fun = f;
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~EventHandler2()	{
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
		if (fun)
			fun();
	}

protected:
	boost::function<void()> fun;
};

}

#endif // EVENTHANDLER2_HPP
