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




/*!
 * \class ContinousExecutor
 * \brief Call step continously.
 * This object calls step method from it's main component continously.
 */
class ContinousExecutor : public Executor {
public:

	ContinousExecutor(const std::string & n) : Executor(n) {};

	virtual ~ContinousExecutor() {
		double spl = elapsed/loops;
		double lps = 1.0 / spl;
		LOG(NOTICE) << "Executor " << name() << " finished.\n\tDid " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
	}

	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
		max_iter = pt.get("iterations", -1);
		mk_name = pt.get("main_component", "");
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run();

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main component - it's step method will be called in each loop
	Base::Component * main_component;

	/// Main component name
	std::string mk_name;


	double elapsed;
	int loops;
};



/*!
 * \class PassiveExecutor
 * \brief Don't call any step, only react.
 * This object doesn't call any methods from it's components explicitly,
 * but only process events and calls connected handlers.
 */
class PassiveExecutor : public Executor {
public:
	PassiveExecutor(const std::string & n) : Executor(n) {};

	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run();

private:
};



/*!
 * \class PeriodicExecutor
 * \brief Execute component periodically.
 * This object calls step method from it's main component periodically with given interval.
 */
class PeriodicExecutor : public Executor {
public:
	PeriodicExecutor(const std::string & n) : Executor(n) {};

	virtual ~PeriodicExecutor() {
		double spl = elapsed/loops;
		double lps = 1.0 / spl;
		LOG(NOTICE) << "Executor " << name() << " finished.\n\tDid " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
	}

	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
		max_iter = pt.get("iterations", -1);
		mk_name = pt.get("main_component", "");
		interval = pt.get("interval", 1.0);
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run();

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main component - it's step method will be called in each loop
	Base::Component * main_component;

	/// Main component name
	std::string mk_name;

	/// Periodic mode interval in seconds
	float interval;

	double elapsed;
	int loops;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
