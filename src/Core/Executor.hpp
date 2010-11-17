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
#include "Props.hpp"
#include "Logger.hpp"

namespace Base {
	class Component;
}

namespace Core {

/*!
 * \class Executor
 * \brief Executor object holds \ref Base::Component "components" and implements message queue.
 *
 * Executor is only interface for concrete implementations. These implementations
 * can differ in the way components are managed etc.
 *
 * \author mstefanc
 */
class Executor : public Common::Thread, public Base::Props {
public:

	Executor(const std::string & n) : running(false), paused(true), name_(n) {
	}

	virtual ~Executor() {
	}

	/*!
	 * Add new Component to Executor.
	 * \param name name of component
	 * \param component component to be added to executor
	 */
	void addComponent(const std::string & name, Base::Component * component) {
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

	void restart() {
		paused = false;
		if (!running)
			start();
	}

	void pause() {
		paused = true;
	}

	/*!
	 * Finish main Executor loop thus ending associated thread.
	 */
	void finish() {
		running = false;
	}

	/*!
	 * Save configuration
	 */
	void save(ptree & pt) {
	}

	/*!
	 * Return name
	 */
	const std::string & name() const {
		return name_;
	}

protected:

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

	/// List of components managed by this Executor
	std::map<std::string, Base::Component *> components;

	/// Flag indicating that executor is running
	volatile bool running;

	/// Flag indicating that executor is paused
	volatile bool paused;

	/// FIFO queue for incoming events
	std::deque<Base::EventHandlerInterface *> queue;

	std::deque<Base::EventHandlerInterface *> loc_queue;

	/// Name of execution thread
	std::string name_;

	/// Asynchronous event queue synchronization
	boost::mutex mtx;

	///
	boost::mutex ev_mtx;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
