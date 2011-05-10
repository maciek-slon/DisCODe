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
#include "EventHandler.hpp"
#include "Logger.hpp"

namespace Base {
	class Component;
}

namespace Core {

enum ExecutorState {
	Loaded,
	Paused,
	Starting,
	Running,
	Pausing,
	Finished
};

/*!
 * \class Executor
 * \brief Executor object holds \ref Base::Component "components" and implements message queue.
 *
 * Executor is only interface for concrete implementations. These implementations
 * can differ in the way components are managed etc.
 *
 * \author mstefanc
 */
class Executor : public Common::Thread {
public:

	Executor(const std::string & n) :  m_name(n), m_state(Loaded), m_period(2) {
	}

	virtual ~Executor() {
	}

	/*!
	 * Add new Component to Executor.
	 * \param name name of component
	 * \param component component to be added to executor
	 */
	void addComponent(const std::string & name, Base::Component * component, int priority) {
		// TODO: implement priority queue
		if (priority > 0)
			active_components.push_back(component);

		components[name] = component;
	}

	/*!
	 * Queue event handler in internal FIFO buffer
	 */
	void queueEvent(Base::EventHandlerInterface * h) {
		mtx.lock();
		queue.push_back(h);
		mtx.unlock();
	}

	/*!
	 * Returns event handler scheduler for given handler.
	 * \param h event handler to be scheduled
	 * \returns pointer to event handler scheduler
	 */
	Base::EventHandlerInterface * scheduleHandler(Base::EventHandlerInterface * h) {
		Base::EventScheduler<Executor> * handler = new Base::EventScheduler<Executor>();
		handler->setup(this, &Executor::queueEvent);
		handler->registerHandler(h);
		return handler;
	}

	void restart();

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
	 * Return name
	 */
	const std::string & name() const {
		return m_name;
	}


	std::vector<std::string> listComponents();

	/*!
	 *
	 * @param period period in seconds
	 */
	void setPeriod(float period) {
		m_period = period;
	}

	ExecutorState state() const {
		return m_state;
	}

protected:

	bool ensureState(ExecutorState st, const std::string & errmsg);

	/*!
	 * Implementation of run method from Thread.
	 */
	void run();

	/**
	 * Execute all pending events.
	 */
	void executeEvents() {
		mtx.lock();
		loc_queue = queue;
		queue.clear();
		mtx.unlock();

		while (!loc_queue.empty()) {
			Base::EventHandlerInterface * h;
			h = loc_queue.front();
			loc_queue.pop_front();
			h->execute();
		}
	}


	typedef std::pair<std::string, Base::Component*> ComponentPair;

	/// List of components managed by this Executor
	std::map<std::string, Base::Component *> components;


	/// List of components managed by this Executor
	std::vector<Base::Component *> active_components;

	/// FIFO queue for incoming events
	std::deque<Base::EventHandlerInterface *> queue;

	std::deque<Base::EventHandlerInterface *> loc_queue;

	/// Name of execution thread
	std::string m_name;

	/// Asynchronous event queue synchronization
	boost::mutex mtx;

	///
	boost::mutex ev_mtx;

	boost::condition_variable m_cond;
	boost::mutex m_cond_mtx;

	ExecutorState m_state;

	float m_period;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
