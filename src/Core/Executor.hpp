/*!
 * \file Executor.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-05-02
 */

#ifndef EXECUTOR_HPP_
#define EXECUTOR_HPP_

#include <queue>
#include <map>

#include "Thread.hpp"

namespace Base {
	class Component;
	class EventHandlerInterface;
}

namespace Core {

/*!
 * Possible Executor states.
 */
enum ExecutorState {
	Loaded,   //!< Loaded
	Pausing,  //!< Pausing
	Paused,   //!< Paused
	Starting, //!< Starting
	Running,  //!< Running
	Finishing,//!< Finishing
	Finished  //!< Finished
};

/*!
 * \class Executor
 * \brief Executor object holds \ref Base::Component "components" and implements message queue.
 *
 * TODO: Finish documentation
 */
class Executor : public Common::Thread {
public:

	/*!
	 * Initialize all member variables.
	 *
	 * By default, Executor has period set to 0, and after
	 * creation its state is set to Loaded.
	 *
	 * @param n name of created Executor
	 */
	Executor(const std::string & n);

	/*!
	 * Destroy executor.
	 *
	 * Executor is not owner of components it's holding,
	 * so none of them are released at this point.
	 */
	virtual ~Executor();

	/*!
	 * Add new Component to Executor.
	 * \param name name of component
	 * \param component component to be added to executor
	 */
	void addComponent(const std::string & name, Base::Component * component, int priority);

	/*!
	 * Queue event handler in internal FIFO buffer
	 */
	void queueEvent(Base::EventHandlerInterface * h);

	/*!
	 * Returns event handler scheduler for given handler.
	 * \param h event handler to be scheduled
	 * \returns pointer to event handler scheduler
	 */
	Base::EventHandlerInterface * scheduleHandler(Base::EventHandlerInterface * h);

	/*!
	 *
	 */
	void restart();

	/*!
	 * Pause Executor.
	 *
	 * After calling this method, state is set to Pausing, and only after
	 * Executor actually pause, state is set to Paused.
	 */
	void pause();

	/*!
	 * Initialize all managed components.
	 */
	void initialize();

	/*!
	 * Reset execution thread, making it possible to start over again
	 * (reinitialize components etc.).
	 */
	void reset();

	/*!
	 * Finish main Executor loop thus ending associated thread.
	 */
	void finish();

	/*!
	 * Return name of Executor
	 */
	const std::string & name() const {
		return m_name;
	}

	/*!
	 * Return names of all components handled in this Executor.
	 *
	 * @return vector containing names of components
	 */
	std::vector<std::string> listComponents();

	/*!
	 *
	 * @param period period in seconds
	 */
	void setPeriod(float period) {
		m_period = period;
	}

	/*!
	 *
	 * @return state of Executor
	 */
	ExecutorState state() const {
		return m_state;
	}

protected:

	/*!
	 * Check if state is as expected, and print message if it's not.
	 *
	 * @param st expected state
	 * @param errmsg message to print in case of unexpected state
	 *
	 * @return true if state() == st, false otherwise
	 */
	bool ensureState(ExecutorState st, const std::string & errmsg);

	/*!
	 * Implementation of run method from Thread.
	 */
	void run();

	/**
	 * Execute all pending events.
	 */
	void executeEvents();

	typedef std::pair<std::string, Base::Component*> ComponentPair;
	typedef std::pair<int, Base::Component*> ComponentPriority;

	/// List of components managed by this Executor
	std::map<std::string, Base::Component *> components;

	/// List of components managed by this Executor
	std::vector<ComponentPriority> active_components;

	/// FIFO queue for incoming events
	std::deque<Base::EventHandlerInterface *> queue;

	std::deque<Base::EventHandlerInterface *> loc_queue;

	/// Name of execution thread
	std::string m_name;

	/// Asynchronous event queue synchronization
	boost::mutex mtx;

	///
	boost::condition_variable m_event_cond;
	boost::mutex m_event_cond_mtx;

	boost::condition_variable m_cond;
	boost::mutex m_cond_mtx;

	/// current state
	ExecutorState m_state;

	/// period
	float m_period;

	/// time of next wake-up, used when period > 0
	boost::system_time next_wakeup;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
